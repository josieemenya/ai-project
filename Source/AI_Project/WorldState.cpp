// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldState.h"

bool FWorldState::operator==(const FWorldState& Other) const
{
	return StateValues.OrderIndependentCompareEqual(Other.StateValues);
}

bool FWorldState::Satisfies(const FWorldState Other) const
{
		for (auto& X : Other.StateValues)
		{
			auto GoalKey = X.Key;
			bool GoalValue = X.Value;
  			
			if (StateValues.Find(GoalKey) == nullptr)
				return false;
			if (*StateValues.Find(GoalKey) != GoalValue)
				return false;
		}
		return true;
}

