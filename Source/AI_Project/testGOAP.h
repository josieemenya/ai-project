// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"

/**
 * 
 */
 
//USTRUCT(BlueprintType)




struct FGOAPState
{
	//GENERATED_BODY()
	TMap<FString, bool> StateValues;
};

struct FWorldState 
{
	TMap<FString, bool> StateValues;

	bool Satisfies(FWorldState* Comparision)
	{
		for (auto &Pair : Comparision->StateValues)
		{
			if (!StateValues.Contains(Pair.Key) || StateValues[Pair.Key] != Pair.Value)
			{
				return false;
			}
		}
		return true;
	}
};

struct FGOAPAction
{
	//GENERATED_BODY()
	FString Name;
	TMap<FString, bool> Preconditions;
	TMap<FString, bool> Effects;
	int32 Cost;

	FGOAPAction() : Cost(1) {}
	TFunction<void()> Perform;

	float Duration;
	bool bIsRunning;

	FWorldState* OwnBeliefs;
	bool isAchievable() const {return true; }
	bool isAchievableGiven(FWorldState* CurrentState)
	{
		for (auto &Pre : Preconditions)
		{
			if (!CurrentState->StateValues.Contains(Pre.Key) || CurrentState->StateValues[Pre.Key] != Pre.Value)
			{
				return false;
			}
		}
		return true;
	}
};

struct FGOAPGoal
{
	//GENERATED_BODY()
	FString Name;
	int32 Priority;
	TMap<FString, bool> DesiredState;
	bool isSatisfiedBy(FWorldState* CurrentState)
	{
		for (auto &Pair : DesiredState)
		{
			if (!CurrentState->StateValues.Contains(Pair.Key) || CurrentState->StateValues[Pair.Key] != Pair.Value)
			{
				return false;
			}
		}
		return true;
	}
};

struct Node
{
	Node* Parent;
	float Cost;
	FWorldState* State;
	FGOAPAction* Action;
};

class AI_PROJECT_API testGOAP
{
public:
	FGOAPGoal* CurrentGoal;

	static TArray<FGOAPAction*> Plan(TArray<FGOAPAction*> AvailableActions, FWorldState* CurrentState, FGOAPGoal* Goal);
	static bool BuildGraph(Node* Parent, TArray<Node*>& Leaves, TArray<FGOAPAction*> UsableActions, FGOAPGoal* Goal);
	testGOAP();
	~testGOAP();
};
