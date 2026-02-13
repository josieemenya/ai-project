// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "testGOAP.h"
#include "GameFramework/Character.h"
#include "FSM.h"
#include "LockedRoomGhost.generated.h"

class AAIController; 

// noteds for opening a locked door in ue5
// go to Target
// notiuce door is locked
// search for key 
// got to door again unlock door
// activate nav link
// fcontiue to target

class ADoor;
class AKiey;
UCLASS()
class AI_PROJECT_API ALockedRoomGhost : public ACharacter, public IFSM
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALockedRoomGhost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FGOAPAction*> AvailableActions;
	FGOAPAction* CurrentAction;

	FGOAPGoal* CurrentGoal;
	TArray<FGOAPGoal*> Goal;

	FSMAction* FSMCurrentAction;	
	EAIState AvailabeStates;
	IFSM* FiniteMachine;
	void FSMUpdate();

	void MoveToDoor();

	ADoor* Door;
	AKiey* DoorKey; 
	FWorldState* WorldState;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
