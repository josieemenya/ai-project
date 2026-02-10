// Fill out your copyright notice in the Description page of Project Settings.


#include "FiniteMachine.h"

FiniteMachine::FiniteMachine()
{
}

FiniteMachine::~FiniteMachine()
{
}

void FiniteMachine::Update()
{
	switch (State) // you can do it like this.
	{
		case 1:
			// do something
			break;

		case 2:
			// do something else
			break;
	}

	// but it makes more sense to use if statements, since you can have more complex conditions.


	if ((State & EFriendly::Hostile) != 0) // if hostile, then do something
	{
		OnHostile();
	}

	else if ((State & EFriendly::Friendly) != 0) // if friendly, then do something else
	{
		OnFriendly();
	}

	
}