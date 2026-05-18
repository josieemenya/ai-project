// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldState.h"
#include "Components/ActorComponent.h"
#include "UtilityWorldSystem.generated.h"

/**
 * 
 */

class UBlackboardComponent;
class UGoal;
class AAIController;
class UPlannerComponent;

UCLASS(Blueprintable)
class UUtilityReasoner : public UActorComponent
{
	GENERATED_BODY()
public:
	
	void Initialize(AAIController* InController, UPlannerComponent* InPlanner);

	void StartThinking();

	UFUNCTION()
	void HandlePlanFinished();

	UFUNCTION()
	void HandlePlanInvalid();

	UPROPERTY(BlueprintReadWrite, Category = "GOAP")
	FWorldState InternalAgentState; 
	
protected:
	virtual void BeginPlay() override;

	void Think();

	UGoal* ChooseBestGoal();

	void RequestPlan(UGoal* Goal);
	
	

protected:

	UPROPERTY(EditAnywhere, Category="Utility AI")
	TArray<TSubclassOf<UGoal>> GoalClasses;

	UPROPERTY()
	TArray<UGoal*> Goals;

	UPROPERTY()
	TObjectPtr<UGoal> CurrentGoal;

	UPROPERTY()
	TObjectPtr<UPlannerComponent> Planner;

	UPROPERTY()
	TObjectPtr<AAIController> OwnerController;

	FTimerHandle ThinkTimer;
	
	UFUNCTION(BlueprintNativeEvent, Category="Utility AI", meta = (ToolTip = "This function is called to make up current world state. Please define and add any variables you want to be considered in the plan"))
	void SyncToWorldState(FWorldState CurrentWorldState); 
};

UCLASS()
class DESIGNER_FRIENDLY_GOAP_API UUtilityWorldSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	public:
	
	UGoal* ScoreAndChooseGoals(TArray<UGoal*>& InstancedGoals, UBlackboardComponent* Blackboard);
	
};
