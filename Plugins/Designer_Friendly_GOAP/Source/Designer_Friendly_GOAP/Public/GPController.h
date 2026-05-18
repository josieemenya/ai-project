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
class DESIGNER_FRIENDLY_GOAP_API AGPController : public AAIController
{
	GENERATED_BODY()
	
public:
	
	AGPController();
	
	UPROPERTY(VisibleAnywhere, Category="GP")
	UAIPerceptionComponent* PerceptionComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GP")
	class UPlannerComponent* Planner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GP")
	TArray<TSubclassOf<UGoal>> Goals;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GP")
	TArray<UGoal*> InstancedGoals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GP")
	UGoal* CurrentGoal; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GP")
	FWorldState CurrentState;
	
	// should i mark with UFUNCTION() ? leaning towards no.
	UGoal* GetBestGoal(); 
	
	UFUNCTION(BlueprintCallable, Category="GP")
	void StartPlanning();
	
	UFUNCTION(BlueprintCallable, Category="GP")
	void Replan(); 
	
	void InstantiateGoals(); 
	
	void UpdateActions(); // called every tick to update the action stack, if the stack is empty, call the planner to generate a new plan based on the current world state and the desired goal state, then execute the first action in the stack and remove it from the stack

	bool RegisterSeePlayer(UPlannerComponent* InPlanner);
	
	
	UFUNCTION()
	void OnCharacterDeathAnim(AAIController* ParentController); 

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GP")
	bool ShouldInterruptCurrentPlan;

	

protected:
	//void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
};
