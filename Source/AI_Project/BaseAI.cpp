// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAI.h"


// Sets default values
ABaseAI::ABaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateActions();

}

void ABaseAI::UpdateActions()
{
	if (ActionStack.IsEmpty())
		return;

	// (hopefully) sets current action to the top of the stack, then pops it off the stack so that the next action will be at the top of the stack for the next tick, this is assuming that the actions are executed immediately and do not take multiple ticks to complete, if they do take multiple ticks to complete, then we will need to keep track of the current action and only pop it off the stack when it is completed, this can be done by adding a boolean variable to the FAIAction struct that indicates whether the action is currently being executed or not, and only popping it off the stack when it is completed
	CurrentAction = ActionStack.Pop();

	this->CurrentAction.DoAction();

	
}

// Called to bind functionality to input
void ABaseAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

