// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RHIValidationCommon.h"

/**
 * 
 */

struct FState; 
 
 struct FAction
 {
 	FName ActionName;
 	TFunction<void()> Action;
 };

struct FCondition
{
	virtual bool test() = 0;
};
// add in float condition, bool condition, etc
// not, and,  or condition


struct FTransition
{
	FCondition* Condition;
	bool isTriggered() { return Condition->test(); } ;

	FState* TargetState;
	FAction* Action;
};

struct FState
{
 	FName StateName;
 	TArray<FAction*> StateAction;

 	FAction* EntryAction;
 	FAction* ExitAction;

 	TArray<FTransition*> Transitions;
};





 
class AI_PROJECT_API UStateMachine
{
public:
	UStateMachine();
	~UStateMachine();

	TArray<FState*> States;
	FState* CurrentState;

	TArray<FAction*> Update();
};
