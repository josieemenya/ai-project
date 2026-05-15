// Fill out your copyright notice in the Description page of Project Settings.


#include "GPController.h"

#include "AI_ProjectCharacter.h"
#include "BrainComponent.h"
#include "Damage.h"
#include "PlannerComponent.h"
#include "PrisonManagementSystem.h"
#include "PrisonRules.h"
#include "Routine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/SceneCapture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/Character.h"

AGPController::AGPController()
{
	Planner = CreateDefaultSubobject<UPlannerComponent>("PlannerComponent");
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComp");
	DamageComp = CreateDefaultSubobject<UDamage>(TEXT("DamageComp"));
	RoutineComp = CreateDefaultSubobject<URoutineComponent>(TEXT("RoutineComp"));
	PrimaryActorTick.bCanEverTick = true;
}

UGoal* AGPController::GetBestGoal()
{
	UGoal* bestGoal = nullptr;
	float BestScore = -1;

	for (UGoal* Goal : InstancedGoals)
	{
		float utility = Goal->GetUtility(Blackboard);

		UE_LOG(LogTemp, Warning, TEXT("Evaluating Goal: %s"), *Goal->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s Goal Value: %f"), *Goal->GetName(), utility);


		if (utility < 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Goal Utility less than zero, Goal Name: %s, Goal Value: %f"), *Goal->GetName(),
			       utility);
		}

		if (BestScore < utility)
		{
			bestGoal = Goal;
			BestScore = utility;
		}
	}

	return bestGoal;
}

void AGPController::StartPlanning()
{
	SetActorTickEnabled(true);
	if (Planner->ToDoStack.Num() > 0) // don't plan of we have a task
	{
		UE_LOG(LogTemp, Warning, TEXT("TASK TASK TASK"));
		return;
	}


	if (!Goals.IsEmpty()) // if we have goals
	{
		// then set current state to the top most goal

		UE_LOG(LogTemp, Warning, TEXT("Updating Current Goal."));

		

		RegisterSeePlayer(Planner); // if can see player update black board to reflect that
		
		//SyncWorldState(CurrentState, Blackboard); 
		
		FWorldState BaseCurrentState = CurrentState;

		Planner->UpdateSmartObjects(CurrentState);

		CurrentGoal = GetBestGoal();

		if (!CurrentGoal) return;

		UE_LOG(LogTemp, Warning, TEXT("Goal class: %s"), *CurrentGoal->GetName());


		if (CurrentGoal->bRequiresSmartObject && Planner->AllSmartObjectsNearby.Num() == 0)
		// if goal needs to interact with smart obj
		{
			//UE_LOG(LogTemp, Warning, TEXT("Skipping planning: no smart objects yet"));
			return;
		}
		
		if (Planner->CurrentAction)
		{
			return;
		}

		// StartPlanning for real
		Planner->PlanGoal(BaseCurrentState, CurrentGoal->DesiredState);
		//UE_LOG(LogTemp, Warning, TEXT("Plan size after planning: %d"), Planner->ToDoStack.Num()); 


		for (auto& Pair : CurrentGoal->DesiredState.StateValues)
		{
			UE_LOG(LogTemp, Warning, TEXT("Goal requires: %s = %s"),
			       *Pair.Key,
			       Pair.Value ? TEXT("true") : TEXT("false"));
		}

		if (Planner->ToDoStack.Num() > 0) // if we have a sequence of actions we have a goal so 
		{
			// remove top goa;, although i'm thinking i should just sink it instead
			auto LastGoal = Goals[0];
			Goals.Remove(0);
			Goals.Push(LastGoal);
		}
	}
}

void AGPController::Replan()
{
	if (Planner->ToDoStack.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("TASK TASK"));
		return;
	}

	Planner->UpdateSmartObjects(CurrentState);

	if (Goals.Num() > 0)
	{
		StartPlanning();
		//UGoal* Curren
	}
}

void AGPController::InstantiateGoals()
{
	for (TSubclassOf<UGoal> GoalClass : Goals)
	{
		InstancedGoals.Add(NewObject<UGoal>(this, GoalClass));
	}
}

void AGPController::UpdateActions()
{
	Planner->UpdateStack(GetPawn());
}

bool AGPController::RegisterSeePlayer(UPlannerComponent* MyPlanner)
{
	TArray<AActor*> SeeActors;

	if (auto HasPerception = FindComponentByClass<UAIPerceptionComponent>())
	{
		HasPerception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SeeActors);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find perception component"));
	}

	if (SeeActors.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Registering SeeActors"));
		auto FoundPlayer = SeeActors.Find(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (FoundPlayer != INDEX_NONE)
		{
			Blackboard->SetValueAsBool("HasSeenPlayer", true);
			auto PlayerinWorld = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			Blackboard->SetValueAsObject("Player", PlayerinWorld);
			ShouldInterruptCurrentPlan = true;

			if (GetPawn() && PlayerinWorld)
			{
				float Dst = (GetPawn()->GetActorLocation() - PlayerinWorld->GetActorLocation()).Length();
				Blackboard->SetValueAsFloat("DistanceToPlayer", Dst);
				ShouldInterruptCurrentPlan = true;
			}

			return true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Perceived Jack Shit"));
		Blackboard->SetValueAsBool("HasSeenPlayer", false);
		Blackboard->SetValueAsFloat("DistanceToPlayer", -1);
		ShouldInterruptCurrentPlan = true;
	}

	return false;
}

void AGPController::SyncWorldState_Implementation(FWorldState State, UBlackboardComponent* BBlackboard)
{
	// 
	
	
	CurrentState.StateValues.FindOrAdd("HasPlayer", IsValid(BBlackboard->GetValueAsObject("Player")));
	CurrentState.StateValues.FindOrAdd("CloseToPlayer", BBlackboard->GetValueAsFloat("DistanceToPlayer") <= 30.f);
	CurrentState.StateValues.FindOrAdd("KnockedOut", BBlackboard->GetValueAsBool("KnockedOut"));
	CurrentState.StateValues.FindOrAdd("GoodHealth", BBlackboard->GetValueAsFloat("Health") >= 45.f);
	CurrentState.StateValues.FindOrAdd("Engaged", BBlackboard->GetValueAsBool("Engaged"));
	CurrentState.StateValues.FindOrAdd("InCombat", BBlackboard->GetValueAsBool("InCombat"));
	CurrentState.StateValues.FindOrAdd("HasTarget", IsValid(BBlackboard->GetValueAsObject("Target")));
	CurrentState.StateValues.FindOrAdd("HasSignal", BBlackboard->GetValueAsBool("SeenSignal"));
	CurrentState.StateValues.FindOrAdd("IsInvestigating", BBlackboard->GetValueAsBool("Investigating"));

}

void AGPController::OnCharacterDeathAnim(AController* ParentController)
{
	ACharacter* ParentCharacter = Cast<ACharacter>(ParentController->GetOwner());

	if (!ParentCharacter) return;

	USkeletalMeshComponent* Mesh = ParentCharacter->GetMesh();

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

	if (AnimInstance)
	{
		//AnimInstance->Montage_Play() -- basic gist
		// make them immobile for like, 15 mins?
		// make them unable to move
		this->BrainComponent->PauseLogic("KnockOut");
		ShouldInterruptCurrentPlan = true;
		// maybe do while loop but, after knockout 
		this->BrainComponent->ResumeLogic("nOT kNOCKED OUT");
	}
}

void AGPController::BeginPlay()
{
	Super::BeginPlay();

	InstantiateGoals();

	check(Planner);
	check(Blackboard);

	if (!Planner)
	{
		// log returb, same with blackboard
	}

	Blackboard->SetValueAsFloat("Health", DamageComp->CharacterMaxHealth);

	Planner->OnPlanInvalid.AddUObject(this, &AGPController::Replan);
	GetWorldTimerManager().SetTimerForNextTick([this]()
	{
		Planner->UpdateSmartObjects(CurrentState);
		StartPlanning();
	});
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AGPController::OnTargetPerceptionUpdated);
	DamageComp->OnCharacterDeath.AddDynamic(this, &AGPController::OnCharacterDeathAnim);
}

void AGPController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (Planner->ToDoStack.Num() > 0) // goal
	{
		if (ShouldInterruptCurrentPlan)
		{
			SyncWorldState(CurrentState, Blackboard); 
			UGoal* NewGoal = GetBestGoal();

			if (NewGoal != CurrentGoal)
			{
				Planner->ToDoStack.Empty();
				Planner->CurrentAction = nullptr;
				CurrentGoal = NewGoal;
				StartPlanning();
			}
			ShouldInterruptCurrentPlan = false;
		}
		
		UpdateActions();
		return;
	}

	if (Goals.Num() > 0)
	{
		StartPlanning();
	}

	for (auto I : InstancedGoals)
	{
		if (I)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Goal Name: %s, Score: %f"), *I->Name, I->GetUtility(Planner->BB_Planner));
		}
	}
}

void ExtractDataToBlackboard(UBlackboardComponent& Blackboard, const ASignal& Signal)
{
	const FSignalData& signalData = Signal.SignalData; 
	for (ACharacter* SeenCharacter : signalData.InvolvedCharacters)
	{
		if (SeenCharacter->IsA(AAI_ProjectCharacter::StaticClass()))
		{
			Blackboard.SetValueAsObject("Player", SeenCharacter); 
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("AI Signal saw a character that was not a player : %s. Please implement."), *SeenCharacter->GetName())
		}
	}
}

void AGPController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	
	for (auto x : Blackboard->GetBlackboardAsset()->Keys)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Key: %s, Value: %s"), *x.EntryName.ToString(), *Blackboard->GetValueAsObject(x.EntryName)->GetName())
	}
	if (Actor)
	{
		check (Blackboard)
		if (AAI_ProjectCharacter* PC = Cast<AAI_ProjectCharacter>(Actor))
		{
			Blackboard->SetValueAsObject("Player", PC);
			Blackboard->SetValueAsVector("LastKnownPlayerLocation", PC->GetActorLocation());

			if (Stimulus.WasSuccessfullySensed())
			{
				if (PC->CurrentlyHoldingItem)
				{
					if (PC->HeldInvItem.ItemTypes.Contains(EItemType::CONTRABAND) || PC->HeldInvItem.ItemTypes.Contains(
						EItemType::WEAPON))
					{
						FSignalData VisiblyArmed = FSignalData();
						VisiblyArmed.InvolvedCharacters.Add(PC);
						VisiblyArmed.ActionType = EActionType::CONTRABAND;
						VisiblyArmed.SignalLifeSpan = 10.f;
						VisiblyArmed.StimulusLocation = PC->GetActorLocation();

						GetWorld()->GetSubsystem<USignalManagement>()->ActivateSignal(
							VisiblyArmed, PC->GetActorLocation());
						Blackboard->SetValueAsBool("SeenSignal", true);
						ShouldInterruptCurrentPlan = true;
					}
				}
			}
		}
		if (ASignal* Signal = Cast<ASignal>(Actor))
		{
			Blackboard->SetValueAsBool("SeenSignal", true);
			GetBlackboardComponent()->SetValueAsObject(FName("Target"), Actor);
			GetBlackboardComponent()->SetValueAsBool(FName("Engaged"), true);
			ExtractDataToBlackboard(*Blackboard, *Signal); 
			ShouldInterruptCurrentPlan = true;
		}
		else
		{
			GetBlackboardComponent()->SetValueAsBool("SeenSignal", false);
			GetBlackboardComponent()->SetValueAsBool(FName("Engaged"), false);
		}
	}
	
}
