// Fill out your copyright notice in the Description page of Project Settings.


#include "GPController.h"

#include "AI_ProjectCharacter.h"
#include "Damage.h"
#include "PlannerComponent.h"
#include "Routine.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	float BestScore = INT_MIN;
	
	for (UGoal* Goal : InstancedGoals)
	{
		
		float utility = Goal->GetUtility(Planner->BB_Planner);
		
		UE_LOG(LogTemp, Warning, TEXT("Evaluating Goal: %s"), *Goal->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s Goal Value: %f"), *Goal->GetName(), utility);
		
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
		return;
	}
	
	
	if (!Goals.IsEmpty()) // if we have goals
	{
		// then set current state to the top most goal
		
		UE_LOG(LogTemp, Warning, TEXT("Updating Current Goal.")); 
		
		FWorldState BaseCurrentState = CurrentState; 
		
		RegisterSeePlayer(Planner); // if can see player update black board to reflect that
	
		Planner->UpdateSmartObjects(BaseCurrentState); 
			
		CurrentGoal = GetBestGoal(); 
		
		if (!CurrentGoal) return;
		
		UE_LOG(LogTemp, Warning, TEXT("Goal class: %s"), *CurrentGoal->GetName());
		
		
		if (CurrentGoal->bRequiresSmartObject && Planner->AllSmartObjectsNearby.Num() == 0) // if goal needs to interact with smart obj
		{
        	UE_LOG(LogTemp, Warning, TEXT("Skipping planning: no smart objects yet"));
			return;
		}
		
		// StartPlanning for real
		Planner->PlanGoal(BaseCurrentState, CurrentGoal->DesiredState);
		UE_LOG(LogTemp, Warning, TEXT("Plan size after planning: %d"), Planner->ToDoStack.Num()); 
		

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
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find perception component"));
	}
	
	if (SeeActors.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Registering SeeActors"));
		auto FoundPlayer = SeeActors.Find(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		if (FoundPlayer != INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can See Player"));
			CurrentState.StateValues.FindOrAdd("Player", true); 
			
			auto PlayerinWorld = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); 
			MyPlanner->BB_Planner->SetValueAsObject("Player", PlayerinWorld);
			return true; 
		}
	
	}
	
	UE_LOG(LogTemp, Warning, TEXT("No player found"));
	
	return false;
}

void AGPController::BeginPlay()
{
	Super::BeginPlay();
	
	InstantiateGoals(); 
	
	//
	Planner->BB_Planner->SetValueAsFloat("Health", DamageComp->CharacterMaxHealth); 
	
	Planner->OnPlanInvalid.AddUObject(this, &AGPController::Replan);
	GetWorldTimerManager().SetTimerForNextTick([this](){
		Planner->UpdateSmartObjects(CurrentState);
		StartPlanning();
	});
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AGPController::OnTargetPerceptionUpdated);
}

void AGPController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Planner->ToDoStack.Num() > 0) // goal
	{
		UpdateActions();
		return;
	}
	
	if (Goals.Num() > 0)
	{
		StartPlanning();
	}
}

void AGPController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// definitely not correct
	
	if (Actor)
	{
		if (Cast<AAI_ProjectCharacter>(Actor))
		{
			Planner->BB_Planner->SetValueAsVector("LastKnownPlayerLocation", Stimulus.StimulusLocation); 
		
			if (!Stimulus.WasSuccessfullySensed())
			{
				UE_LOG(LogTemp, Warning, TEXT("LostSightofPlauyer"));
			}
		}
	}
}
