// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldState.h"

bool FWorldState::operator==(const FWorldState& Other) const
{
	return StateValues.OrderIndependentCompareEqual(Other.StateValues);
}

bool FWorldState::Satisfies(const FWorldState& Other)
{
	for (const auto& State : Other.StateValues)
	{
		auto GoalKey = State.Key;
		auto GoalValue = State.Value;
		
		auto InKey = StateValues.Find(GoalKey);
		
		if (!InKey)
			return false;
		
		if (! Visit([this](const auto& a, const auto& b)
		{
			return a == b;
		}, *InKey, GoalValue))
		{
			return false; 
		}
		
	}
	return true;
}

