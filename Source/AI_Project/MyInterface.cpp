// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInterface.h"

UStateMachine::UStateMachine()
{
}

UStateMachine::~UStateMachine()
{
}

TArray<FAction*> UStateMachine::Update()
{
	FTransition* TriggeredTransition = nullptr;
	for (auto Transition : CurrentState->Transitions)
	{
		if (Transition->isTriggered())
		{
			TriggeredTransition = Transition;
			break;
		}
	}

	if (TriggeredTransition)
	{
		auto TargetState = TriggeredTransition->TargetState;
		TArray<FAction*> ActionsToExecute;
		ActionsToExecute.Add(CurrentState->ExitAction);
		ActionsToExecute.Add(TriggeredTransition->Action);
		ActionsToExecute.Add(TargetState->EntryAction);

		CurrentState = TargetState;
		return ActionsToExecute;
	}
	return CurrentState->StateAction;
}
