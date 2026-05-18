// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonRules.h"
#include "GPController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Damage.h"
#include "Attack.h"
#include "PlannerComponent.h"
#include "PrisonManagementSystem.h"
#include "TimeSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values for this component's properties


ASignal::ASignal()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
}

void ASignal::BeginPlay()
{
	Super::BeginPlay();
	if (SeeObject)
	{
		SeeObject->RegisterForSense(UAISense_Sight::StaticClass());
		SeeObject->RegisterWithPerceptionSystem();
		SeeObject->bAutoRegister = true;
		GEngine->AddOnScreenDebugMessage(1232, 32.f, FColor::MakeRandomColor(), TEXT("buddd")); 
	}
}

void USignalManagement::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	for (int i = 0; i < POOL_SIZE; i++)
	{
		FActorSpawnParameters SpawnInfo;
		ASignal* NewSignal = GetWorld()->SpawnActor<ASignal>(ASignal::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo); 
		NewSignal->SignalData.ID = i;
		SignalPool.FindOrAdd(NewSignal, ESignalState::IGNORED);
	}
	
	NextID = POOL_SIZE; 
}

void USignalManagement::ActivateSignal(FSignalData Data, FVector Location)
{
	for (TPair<TObjectPtr<ASignal>, ESignalState>& Signal : SignalPool)
	{
		if (Signal.Key->SignalData == Data && Signal.Value == ESignalState::ACTIVE)
		{
			Signal.Value = ESignalState::IGNORED;
			break;
		}
	}
	
	
	for (TPair<TObjectPtr<ASignal>, ESignalState>& Pair : SignalPool)
	{
		if (Pair.Value == ESignalState::IGNORED)
		{
			Data.ID = NextID++; 
			Pair.Key->SignalData = Data;
			Pair.Value = ESignalState::ACTIVE; 
			Pair.Key->SetActorLocation(Location);
			UE_LOG(LogTemp, Warning, TEXT("New Signal: %s"), *Pair.Key.GetName())
			return;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("AllActive Signals are In Use"));
}

void USignalManagement::DeactivateSignal(FSignalData Signal)
{
	for (TPair<TObjectPtr<ASignal>, ESignalState>& NewSignal : SignalPool)
	{
		if (NewSignal.Key->SignalData == Signal && NewSignal.Value == ESignalState::ACTIVE)
		{
			NewSignal.Value = ESignalState::IGNORED;
			NewSignal.Key->SetActorLocation(FVector::Zero());
			return;
		}
	}
}
void USignalManagement::ClearSignalPool()
{
	for (TPair<TObjectPtr<ASignal>, ESignalState>& NewSignal : SignalPool)
	{
		NewSignal.Value = ESignalState::IGNORED;
		NewSignal.Key->SetActorLocation(FVector::Zero());
	}
}


ACharacter* UAnimImpactObject::GetResultingCharacter(USkeletalMeshComponent* MeshComponent)
{
	return nullptr;
}

AActor* UAnimImpactObject::GetActorFromSphereTrace(USkeletalMeshComponent* MeshComponent, FHitResult& HitResult,
                                                   TArray<AActor*>& ActorsToIgnore)
{
	FTransform SocketTransform = MeshComponent->GetSocketTransform(SocketName);

	FVector Start = SocketTransform.GetLocation();
	FVector Forward = SocketTransform.GetRotation().GetForwardVector();
	FVector End = Start + Forward * 100.f;

	// debug line 
	UE_LOG(LogTemp, Warning, TEXT("Socket Name: %s"), *SocketName.ToString());


	AActor* HitActor;


	ActorsToIgnore.Add(MeshComponent->GetOwner());


	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	UKismetSystemLibrary::SphereTraceSingleForObjects(MeshComponent->GetWorld(), // ??? weird formatting
	                                                  Start,
	                                                  End,
	                                                  30.f,
	                                                  ObjectTypes,
	                                                  false,
	                                                  ActorsToIgnore,
	                                                  EDrawDebugTrace::ForDuration,
	                                                  HitResult,
	                                                  true,
	                                                  FLinearColor::MakeRandomColor(),
	                                                  FLinearColor::White,
	                                                  10.f
	);


	HitActor = HitResult.GetActor();

	if (HitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("TRACE HIT"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TRACE MISS"));
	}

	if (HitActor)
	{
		PlayDamageSound(HitActor->GetActorLocation(), HitActor);
		PlayDamageAnim(HitActor, MeshComponent->GetOwner());
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName())
	}
	return HitActor;
}

void UAnimImpactObject::AdjustDamageAndRules(ACharacter* Instigator, ACharacter* OtherInstigator)
{
	if (!Instigator || !OtherInstigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid instigators"));
		return;
	}
	

	AController* Controller = OtherInstigator->GetController();
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("No controller on OtherInstigator"));
		return;
	}

	UDamage* DamageComp = Cast<UDamage>(
		Controller->GetComponentByClass(UDamage::StaticClass())
	);

	if (!DamageComp)
	{
		// because this might be the player character
		DamageComp = Cast<UDamage>(OtherInstigator->GetComponentByClass(UDamage::StaticClass())); 
		
		if (!DamageComp)
		{
			// in cause it isn't the Player Character
			return;
		}
	}
	
	if (DamageComp)
	{
		DamageComp->DamageHealth(10.f);
		// MAKE RuleBreak
		
		FSignalData FightingData = FSignalData();
		FightingData.InvolvedCharacters.Add(Instigator);
		FightingData.InvolvedCharacters.Add(OtherInstigator);
		FightingData.ActionType = EActionType::FIGHTING;
		FightingData.ID = FMath::RandRange(25, 99);
		FightingData.SignalLifeSpan = 7.f; 
		FightingData.StimulusLocation = Instigator->GetActorLocation();
		
		UWorld* World = Instigator->GetWorld();
		
		if (!World)
		{
			return;
		}
		
		
		USignalManagement* Signals = World->GetSubsystem<USignalManagement>();

		if (!Signals)
		{
			return;
		}
		
		Signals->ActivateSignal(FightingData, FightingData.StimulusLocation);
	}
}

void UAnimImpactObject::PlayDamageSound(FVector Location, AActor* HitActor)
{
	if (HitActor && !HitSounds.IsEmpty())
	{
		int RandomIndex = FMath::RandRange(0, HitSounds.Num() - 1);
		UGameplayStatics::PlaySoundAtLocation(HitActor->GetWorld(), HitSounds[RandomIndex], Location);
	}
}

void UAnimImpactObject::PlayDamageAnim(AActor* HitActor, AActor* Instigator)
{
	ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
	ACharacter* InstigatorCharacter = Cast<ACharacter>(Instigator);

	if (!HitActor || !Instigator || !HitCharacter) return;
	

	FVector ToInstigator = (Instigator->GetActorLocation() - HitActor->GetActorLocation()).GetSafeNormal();
	float Product = FVector::DotProduct(HitActor->GetActorForwardVector(), ToInstigator);
	if (Product > 0.5f)
	{
		UAnimMontage* AnimToPlay = Montages["Front"];
		if (AnimToPlay)
		{
			if (HitCharacter)
			{
				if (UAnimInstance* AnimInstance = HitCharacter->GetMesh()->GetAnimInstance())
				{
					auto Result = AnimInstance->Montage_Play(AnimToPlay);
					UE_LOG(LogTemp, Warning, TEXT("Montage result: %f"), Result);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Montage result: No AnimInstance"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Montage result: No AnimToPlay"));
		}
	}
	else if (Product < -0.5f)
	{
		UAnimMontage* AnimToPlay = Montages["Back"];
		if (AnimToPlay)
		{
			if (UAnimInstance* AnimInstance = HitCharacter->GetMesh()->GetAnimInstance())
			{
				auto Result = AnimInstance->Montage_Play(AnimToPlay);
				UE_LOG(LogTemp, Warning, TEXT("Montage result: %f"), Result);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Montage result: %f"), Product);
	}
}


void UAnimImpactObject::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimImpactObject::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}
	
	ACharacter* ResultingCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	
	if (!ResultingCharacter)
	{
		return;
	}
	
	if (UAttack* CharAttack =  Cast<UAttack>(ResultingCharacter->GetComponentByClass(UAttack::StaticClass())))
	{
		CharAttack->SetAttacking(false); 
	}
}

void UAnimImpactObject::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UE_LOG(LogTemp, Warning, TEXT("logging from the west side"));

	ACharacter* ResultingCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	if (ResultingCharacter)
	{
		if (ACharacter* PrisonCharacter = Cast<ACharacter>(GetActorFromSphereTrace(MeshComp, Hit, ActorsToIgnore)))
		{
			//UDamage* DamageComp = Cast<UDamage>(PrisonCharacter->GetComponentByClass(UDamage::StaticClass()));
			AdjustDamageAndRules(ResultingCharacter, PrisonCharacter);

			AGPController* GetPrisonController = Cast<AGPController>(PrisonCharacter->GetController());
			if (GetPrisonController)
			{
				// forward declare if i forgot, might also need to change the name Blackboard
				UBlackboardComponent* Blackboard = GetPrisonController->Planner->BB_Planner;
				Blackboard->SetValueAsBool("bInCombat", true);

				//GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Red, FString::Printf(TEXT("Current Goal : %s"), *GetPrisonController->CurrentGoal->Name));
				//GEngine->AddOnScreenDebugMessage(11, 5.f, FColor::Red, FString::Printf(TEXT("Is AI in Combat : %s"), *UKismetStringLibrary::Conv_BoolToString(Blackboard->GetValueAsBool("InCombat"))));
				
				
			}
		}
	}
}

void URollcall::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	TimerHandle = FTimerHandle();
	
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URollcall::HandleRollCall, 2.f, true);
	MorningRollcall.StartHourRange = 9; 
	MorningRollcall.EndHourRange = 9;
	MorningRollcall.StartMinuteRange = 10;
	MorningRollcall.EndMinuteRange = 20;
	
	EveningRollcall.StartHourRange = 22;
	EveningRollcall.EndHourRange = 23;
	EveningRollcall.StartMinuteRange = 40;
	EveningRollcall.EndMinuteRange = 00;
	
	RollCallLocation = GetDefault<UDataContainerSettings>()->Locations; 
	
	bMissingRollcall = false;

	if(GetWorld()){
		UE_LOG(LogTemp, Warning, TEXT("Inint"));
	} 
}

void URollcall::HandleRollCall()
{
	UTimeSystem* TimeSystem = GetWorld()->GetSubsystem<UTimeSystem>();
	GEngine->AddOnScreenDebugMessage(12323, 1.f, FColor::Yellow, TEXT("Rolling call"));
	if (TimeSystem->WithinTimeRange(TimeSystem->GetTimeData(), MorningRollcall))
	{
		APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		
		if (!PC->GetPawn()){
			return;
		}

		float Dist =  FVector::Dist(PC->GetPawn()->GetActorLocation(), RollCallLocation["MorningRollcall"]);
		
		TArray<AActor*> OverlappingAreas;
		
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAreaObject::StaticClass(), OverlappingAreas);
		
		bool InRolecall = false;
		
		for (AActor* Area : OverlappingAreas)
		{
			if (PC->GetPawn()->IsOverlappingActor(Area))
			{
				InRolecall = true;
			}
		}
		
		if (!InRolecall)
		{
			bMissingRollcall = true;
			FSignalData SignalData = FSignalData();
			
			SignalData.ActionType = EActionType::ROLLCALL; 
			SignalData.ID = 0; 
			SignalData.InvolvedCharacters.Add(Cast<ACharacter>(PC->GetPawn()));
			SignalData.SignalLifeSpan = 5.0f; 
			SignalData.StimulusLocation = PC->GetPawn()->GetActorLocation();

			
			GetWorld()->GetSubsystem<USignalManagement>()->ActivateSignal(SignalData, SignalData.StimulusLocation); 
			GetWorld()->GetSubsystem<UPrisonManagementSystem>()->AddFlag(EPrisonState::LOCKDOWN);
		}
	}
	
	if (TimeSystem->WithinTimeRange(TimeSystem->GetTimeData(), EveningRollcall))
	{
		APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		float Dist =  FVector::Dist(PC->GetPawn()->GetActorLocation(), RollCallLocation["EveningRollcall"]);
		
		if (Dist > 40)
		{
			bMissingRollcall = true;
			
			FSignalData SignalData = FSignalData(); 
			
			SignalData.ActionType = EActionType::ROLLCALL; 
			SignalData.ID = 0; 
			SignalData.InvolvedCharacters.Add(Cast<ACharacter>(PC->GetPawn()));
			SignalData.SignalLifeSpan = 5.0f; 
			SignalData.StimulusLocation = PC->GetPawn()->GetActorLocation();
			
			GetWorld()->GetSubsystem<USignalManagement>()->ActivateSignal(SignalData, SignalData.StimulusLocation); 
			GetWorld()->GetSubsystem<UPrisonManagementSystem>()->AddFlag(EPrisonState::LOCKDOWN);
		}
	}
}

void AWinArea::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ATriggerBox::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor && OtherActor != this && OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		UUserWidget* WinScreen = CreateWidget(GetWorld(), WinScreenClass);
		if (WinScreen)
		{
			WinScreen->AddToViewport(); 
			UGameplayStatics::SetGamePaused(GetWorld(), true); 
		}
	}
}
