// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WorldState.h"
#include "Perception/AIPerceptionTypes.h"
#include "GPController.generated.h"

class UGoal;
class UAIPerceptionComponent; 
/**
 * 
 */
UCLASS()
class AI_PROJECT_API AGPController : public AAIController
{
	GENERATED_BODY()
	
public:
	
	AGPController();
	
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* PerceptionComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPlannerComponent* Planner;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UGoal>> Goals;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UGoal*> InstancedGoals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGoal* CurrentGoal; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWorldState CurrentState; 
	
	// should i mark with UFUNCTION() ? leaning towards no.
	UGoal* GetBestGoal(); 
	
	UFUNCTION(BlueprintCallable)
	void StartPlanning();
	
	UFUNCTION(BlueprintCallable)
	void Replan(); 
	
	void InstantiateGoals();
	
	
	void UpdateActions(); // called every tick to update the action stack, if the stack is empty, call the planner to generate a new plan based on the current world state and the desired goal state, then execute the first action in the stack and remove it from the stack

	bool RegisterSeePlayer(UPlannerComponent* Planner);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus){}

protected:
	//void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
};
