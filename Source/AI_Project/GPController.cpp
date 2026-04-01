// Fill out your copyright notice in the Description page of Project Settings.


#include "GPController.h"

#include "BlackboardSystem.h"
#include "PlannerComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/Character.h"

AGPController::AGPController()
{
	Planner = CreateDefaultSubobject<UPlannerComponent>("PlannerComponent");
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComp");
	PrimaryActorTick.bCanEverTick = true;
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
		
		UGoal* CurrentGoal = NewObject<UGoal>(this, Goals[0]);
		
		if (CurrentGoal->bRequiresSmartObject && Planner->AllSmartObjectsNearby.Num() == 0) // if goal needs to interact with smart obj
		{
        	UE_LOG(LogTemp, Warning, TEXT("Skipping planning: no smart objects yet"));
			return;
		}
		
		// StartPlanning for real
		Planner->PlanGoal(BaseCurrentState, CurrentGoal->DesiredState); 
		
		
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

void AGPController::UpdateActions()
{
	Planner->UpdateStack(GetOwner()); 
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
		auto Player = SeeActors.Find(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		if (Player != INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can See Player"));
			CurrentState.StateValues.FindOrAdd("Player", true); 
			FBlackboardCustomEntry PlayerEntry = FBlackboardCustomEntry(); 
			PlayerEntry = {
				"Player",
				EBlackboardKey::Actor
			};
			PlayerEntry.ActorValue = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); 
			MyPlanner->AIbBlackboardSystem->Blackboard->BlackboardEntries.Add(PlayerEntry);
			MyPlanner->BB_Planner->SetValueAsObject("Player", UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); 
			return true; 
		}
	
	}
	
	UE_LOG(LogTemp, Warning, TEXT("No player found"));
	
	return false;
}

void AGPController::BeginPlay()
{
	Super::BeginPlay();
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
