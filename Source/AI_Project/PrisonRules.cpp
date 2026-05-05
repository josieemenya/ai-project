// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonRules.h"
#include "GPController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Damage.h"
#include "PlannerComponent.h"
#include "PrisonManagementSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties


bool URule::bIsRuleBroken_Implementation(UObject* World, const FRuleContext& Context)
{
	return false;
}

void URule::EstablishRuleBreak_Implementation(AAIController* ResultingController)
{
}

void URuleContainer::AddBrokenRule(const FRuleContext& BrokenRule)
{
	Rules.Add(BrokenRule);
	OnRuleBroken.Broadcast();
}



void URuleContainer::DecideConsequence()
{
	auto PCharacter = Cast<ACharacter>(GetOuter());
	AGPController* PController = Cast<AGPController>(PCharacter->GetController());
	if (!PController)
	{
		return;
	}
	for (FRuleContext& Context : Rules)
	{
		GetWorld()->GetGameInstance()->GetSubsystem<UPrisonManagementSystem>()->OnRuleBreakOccured(Context, PController); 
	}
}

void URuleContainer::BeginPlay()
{
	Super::BeginPlay();
	OnRuleBroken.AddDynamic(this, &URuleContainer::DecideConsequence);   
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
	UDamage* DamageComp = Cast<UDamage>(OtherInstigator->GetController()->GetComponentByClass(UDamage::StaticClass()));
	if (DamageComp)
	{
		DamageComp->DamageHealth(10.f);
		// MAKE RuleBreak


		FRuleContextMultiple Fighting = FRuleContextMultiple();
		Fighting.OffendingCharacter = Instigator;
		Fighting.OtherOffendingCharacter = OtherInstigator;
		Fighting.ActionType = EActionType::FIGHTING;
		Fighting.Location = Fighting.OffendingCharacter->GetActorLocation();
		Fighting.ManagementSystem = GetWorld()->GetGameInstance()->GetSubsystem<UPrisonManagementSystem>(); 

		// add to rule breaking class 

		URuleContainer* ContainerComp = Cast<URuleContainer>(
			Fighting.OffendingCharacter->GetComponentByClass(URuleContainer::StaticClass()));

		if (ContainerComp)
		{
			if (!ContainerComp->Rules.Contains(Fighting))
			{
				ContainerComp->Rules.Add(Fighting);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Rules: AlreadyBroken this rule"));
			}
			UE_LOG(LogTemp, Warning, TEXT("Rules: Fighting"));
			// get the other one 
			URuleContainer* GetOtherContainer = Cast<URuleContainer>(
				Fighting.OtherOffendingCharacter->GetComponentByClass(URuleContainer::StaticClass()));
			if (GetOtherContainer)
			{
				if (!GetOtherContainer->Rules.Find(Fighting))
				{
					GetOtherContainer->Rules.Add(Fighting);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Rules: No ContainerComp, Relevant Actor: %s"),
			       *Fighting.OffendingCharacter->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Rules: NoDmgComp"));
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
