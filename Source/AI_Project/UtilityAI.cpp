// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI.h"
#include "BlackboardSystem.h"

// Sets default values for this component's properties
UUtilityAI::UUtilityAI()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUtilityAI::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUtilityAI::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UUtilityAI::CalculatePriority(UBlackboardCustom* Blackboard)
{
	// based on current world, what should AI do next?
	//Blackboard->BlackboardEntries
	return 0.0f; 
}

