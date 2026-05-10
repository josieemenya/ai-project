// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalSelector.h"

// Sets default values for this component's properties
UGoalSelector::UGoalSelector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGoalSelector::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

UGoal* UGoalSelector::ScoreGoals(const TArray<UGoal*> Goals)
{
	return nullptr;
}


// Called every frame
void UGoalSelector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

