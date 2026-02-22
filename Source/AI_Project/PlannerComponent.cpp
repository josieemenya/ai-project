// Fill out your copyright notice in the Description page of Project Settings.


#include "PlannerComponent.h"

// Sets default values for this component's properties
UPlannerComponent::UPlannerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlannerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlannerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlannerComponent::SetGoal(const FString& Key, const bool Value, FName GoalName)
{
	FPlannerGoal NewGoal;
	FPlannerWorldState GoalState; 
	GoalState.StateValues.Add(Key, Value);
	NewGoal.Name = GoalName.ToString();
	NewGoal.DesiredState = GoalState;
	NewGoal.Priority = 1; 
	
	Goals.Add(NewGoal); 
}

void UPlannerComponent::AddToAvailableActions(FPlannerAction NewAction)
{
		AvailableActions.Add(NewAction);
}
