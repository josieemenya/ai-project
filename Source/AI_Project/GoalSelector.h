// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoalSelector.generated.h"

class UGoal; 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UGoalSelector : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoalSelector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UGoal* ScoreGoals(const TArray<UGoal*> Goals) ; 

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
