// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FSM.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum class EAIState : uint8
{
	NONEAGAIN = 0 UMETA(Hidden),
	NONE = 1 << 0 ,
	IDLE = 1 << 1 ,
	SEARCHING = 1 << 2,
	ATTACKING = 1 << 3,
	DEFENDING = 1 << 4 
};

USTRUCT(BlueprintType)
struct FSMAction
{
	GENERATED_BODY()
	TFunction<void()> DoAction;
	FName ActionName; 
};

ENUM_CLASS_FLAGS(EAIState)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFSM : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AI_PROJECT_API IFSM
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	EAIState CurrentState;
	EAIState AvailableStates;

	TMap<EAIState, FSMAction> StateActions;
	//void UpdateState() -- put in AI
	
	//virtual void ChangeState(EAIState NewState) = 0;
};
