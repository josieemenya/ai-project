// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityWorldSystem.h"

#include "AIController.h"
#include "PlannerComponent.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"


void UUtilityReasoner::Initialize(AAIController* InController, UPlannerComponent* InPlanner)
{
	OwnerController = InController;
	Planner = InPlanner;
	
	for (TSubclassOf<UGoal> GoalClass : GoalClasses)
	{
		if (GoalClass)
		{
			UGoal* GoalInstance = NewObject<UGoal>(this, GoalClass);
			Goals.Add(GoalInstance);
		}
	}
}

void UUtilityReasoner::StartThinking()
{
	GetWorld()->GetTimerManager().SetTimer(
		ThinkTimer,
		this,
		&UUtilityReasoner::Think,
		0.5f,
		true
	);
}

void UUtilityReasoner::HandlePlanFinished()
{
	CurrentGoal = nullptr;
	// start planning
}

void UUtilityReasoner::HandlePlanInvalid()
{
	Planner->AbortPlan();
	Planner->CurrentAction = nullptr;
	
	// start planning again
}

void UUtilityReasoner::BeginPlay()
{
	Super::BeginPlay();
}

void UUtilityReasoner::Think()
{
	if (!Planner)
	{
		return;
	}
	
	if (Planner->HasPlan())
	{
		UGoal* NewGoal = ChooseBestGoal();
		
		if (NewGoal && NewGoal != CurrentGoal)
		{
			Planner->AbortPlan();
		}
		
		return; 
	}
	
	UGoal* BestGoal = ChooseBestGoal(); 
	
	if (!BestGoal)
	{
		return;
	}
	
	CurrentGoal = BestGoal;
	
	RequestPlan(BestGoal);
}

UGoal* UUtilityReasoner::ChooseBestGoal()
{
	if (UWorld* World = GetWorld())
	{
		return World->GetSubsystem<UUtilityWorldSystem>()->ScoreAndChooseGoals(Goals, OwnerController->GetBlackboardComponent());
	}
	
	return nullptr;
}

void UUtilityReasoner::RequestPlan(UGoal* Goal)
{
	if (!Planner || !Goal)
	{
		return;
	}
	
	FWorldState CurrentLiveState = Planner->AgentStateValue; 

	Planner->UpdateSmartObjects(CurrentLiveState);
	
	SyncToWorldState(CurrentLiveState);
	
	TArray<UAction*> GeneratedPlan = Planner->PlanGoal(CurrentLiveState, Goal->DesiredState);
	
	if (GeneratedPlan.IsEmpty())
	{
		HandlePlanInvalid(); 
	}
}

void UUtilityReasoner::SyncToWorldState_Implementation(FWorldState CurrentWorldState)
{
	// take in any values you want refelected in current world
}

UGoal* UUtilityWorldSystem::ScoreAndChooseGoals(TArray<UGoal*>& InstancedGoals, UBlackboardComponent* Blackboard)
{
	if (InstancedGoals.IsEmpty())
	{
		return nullptr; 	
	} 
	
	InstancedGoals.Sort([&](UGoal& a, UGoal& b) -> bool
	{
		return a.GetUtility(Blackboard) > b.GetUtility(Blackboard); 
	}); 
	
	return InstancedGoals[0];
}
