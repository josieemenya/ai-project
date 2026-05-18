// Fill out your copyright notice in the Description page of Project Settings.


#include "GPController.h"
#include "BrainComponent.h"
#include "PlannerComponent.h"
#include "UtilityWorldSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/SceneCapture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

AGPController::AGPController()
{
	Planner = CreateDefaultSubobject<UPlannerComponent>("PlannerComponent");
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComp");
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
		

		if (utility < 0){
			UE_LOG(LogTemp, Error, TEXT("Goal Utility less than zero, Goal Name: %s, Goal Value: %f"), *Goal->GetName(), utility); 			
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
		
		FWorldState BaseCurrentState = CurrentState; 
		
		RegisterSeePlayer(Planner); // if can see player update black board to reflect that
	
		Planner->UpdateSmartObjects(BaseCurrentState); 
		
		CurrentGoal = GetBestGoal(); // switch to
		
		GetWorld()->GetSubsystem<UUtilityWorldSystem>()->ScoreAndChooseGoals(InstancedGoals, Blackboard); 
		// then
		CurrentGoal = InstancedGoals[0]; 
		
		if (!CurrentGoal) return;
		
		UE_LOG(LogTemp, Warning, TEXT("Goal class: %s"), *CurrentGoal->GetName());
		
		
		if (CurrentGoal->bRequiresSmartObject && Planner->AllSmartObjectsNearby.Num() == 0) // if goal needs to interact with smart obj
		{
        	//UE_LOG(LogTemp, Warning, TEXT("Skipping planning: no smart objects yet"));
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
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find perception component"));
	}
	
	if (SeeActors.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Registering SeeActors"));
		
	
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Percieved Nothing"));
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("No player found"));
	
	return false;
}



void AGPController::OnCharacterDeathAnim(AAIController* ParentController)
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

	Planner->OnPlanInvalid.AddDynamic(this, &AGPController::Replan);
	GetWorldTimerManager().SetTimerForNextTick([this](){
		Planner->UpdateSmartObjects(CurrentState);
		StartPlanning();
	});
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AGPController::OnTargetPerceptionUpdated);
	
}

void AGPController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UGoal* NewGoal = GetBestGoal(); 
	
	if (NewGoal != CurrentGoal)
	{
		ShouldInterruptCurrentPlan = true; 
	}
	
	if (Planner->ToDoStack.Num() > 0) // goal
	{
		if (ShouldInterruptCurrentPlan)
		{
			Planner->ToDoStack.Empty();
			Planner->CurrentAction = nullptr;
			StartPlanning();
			ShouldInterruptCurrentPlan = false; 
			return;
		}
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
	
	if (Actor)
	{
		if (auto PC = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			if (Stimulus.WasSuccessfullySensed())
			{

			}
		}
	}
}


