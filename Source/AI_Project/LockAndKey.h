// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlannerComponent.h"

class ABaseAI;
/**
 * 
 */
class AI_PROJECT_API LockAndKey
{
public:
	LockAndKey();
	~LockAndKey();
	FPlannerGoal UnlockDoor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseAI* Owner;
	
	TArray<FPlannerAction> AvailableActions;
	
	void SetGoal();
	void SetActions(); 
	void UpdateGoal(UPlannerComponent* AIPlanner);
	
	FPlannerAction LookForKey;
	FPlannerAction FoundKey;
	FPlannerAction OpenDoor;
	FPlannerAction LeaveRoom;
}; 
