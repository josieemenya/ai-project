// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldState.h"

bool FWorldState::operator==(const FWorldState& Other) const
{
	return StateValues.OrderIndependentCompareEqual(Other.StateValues);
}

bool FWorldState::SatisfiesAll(const FWorldState& Other) const
{
		for (auto& X : Other.StateValues)
		{
			auto GoalKey = X.Key;
			bool GoalValue = X.Value;
			
			auto CurrentValue = StateValues.Find(X.Key);
  			bool Val = (CurrentValue) ? *CurrentValue : false; 
			
			if (GoalValue != Val)
			{
				return false;
			}
		}
		return true;
}

bool FWorldState::SatisfiesAny(const FWorldState& Other) const
{
	for (auto& Pair : Other.StateValues)
	{
		FString OtherKey = Pair.Key;
		bool OtherValue = Pair.Value;
			
		const bool* MyValue = StateValues.Find(OtherKey);
		bool Val = (MyValue) ? *MyValue : false; 
			
		if (OtherValue == Val)
		{
			return true;
		}
	}
	return false;
}
