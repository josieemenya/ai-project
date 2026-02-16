// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Planner.generated.h"


struct FPlannerWorldState
{
	TMap<FString, bool> StateValues;
	FPlannerWorldState() = default;
	FPlannerWorldState (FPlannerWorldState &Other)
	{
		StateValues = Other.StateValues;
	}
	
	bool operator==(const FPlannerWorldState& Other) const
	{
		return StateValues.OrderIndependentCompareEqual(Other.StateValues);
	}
	
	bool Satisfies(const FPlannerWorldState& Other)
	{
		for (auto X : Other.StateValues)
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
};

USTRUCT(BlueprintType)
struct FPlannerAction
{
	GENERATED_BODY()
	FName Name; // the name of the action, used for debugging and identification
	FPlannerWorldState Context; // context needed to perform action, such as target location, target actor, etc.
	TFunction<bool()> DoAction; // perform the action, returns true if action was successful, false otherwise
	FPlannerWorldState Effects; // the effects of the action on the world state, used for planning
	float Cost; // the cost of performing the action, used for planning
	
	FPlannerAction(FPlannerAction &Other)
	{
		Name = Other.Name;
		Context = Other.Context;
		DoAction = Other.DoAction;
		Cost = Other.Cost;
		Effects = Other.Effects;
	}
	
	FPlannerAction() = default;
	bool operator==(const FPlannerAction& Other) const
	{
		return Name == Other.Name; // or whatever defines equality
	}
};

struct FPlannerGoal
{
	FString Name;
	FPlannerWorldState DesiredState; // the desired world state that satisfies the goal
	int32 Priority; // the priority of the goal, used for selecting between multiple goals
	
	bool operator==(const FPlannerGoal& Other) const
	{
		return Name == Other.Name;
	}
};

struct Node
{
	FPlannerWorldState State;
	FPlannerAction Action;
	Node* Parent;
	float gCost, fCost, hCost;
	Node() : State{}, Parent(nullptr), Action{}, gCost(0), fCost(0), hCost(0) {}
	Node(FPlannerWorldState &State) : State(State) {}
	Node(FPlannerWorldState State, FPlannerAction Action, Node* Parent, float gCost, float fCost, float hCost) : State(State), Action(Action), Parent(Parent), gCost(gCost), fCost(fCost), hCost(hCost) {};
	bool operator==(const Node& Other) const
	{
		return Action.Effects == Other.Action.Effects; // compare based on the resulting world state after performing the action
	}
	
	Node(Node &Other)
	{
		State = Other.State;
		Action = Other.Action;
		Parent = Other.Parent;
		gCost = Other.gCost;
		fCost = Other.fCost;
		hCost = Other.hCost;
	}
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlanner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AI_PROJECT_API IPlanner
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:

	TArray<FPlannerAction> PlanGoal(FPlannerWorldState CurrentState, FPlannerWorldState DesiredState);
	TArray<FPlannerAction> BuildPlan(Node* Last);
	TArray<FPlannerAction> AvailableActions; // the actions that the planner can use to achieve goals, this should be populated by the actor that implements the planner interfac
	TArray<FPlannerAction> FilterAvailableActions(TArray<FPlannerAction> Actions, FPlannerWorldState CurrentState);
	TArray<FPlannerAction> GetSatisfyingActions(TArray<FPlannerAction> Actions, FPlannerWorldState DesiredState);
	TArray<FPlannerAction> ToDoStack; 
	FPlannerGoal DesiredGoal; 
	TArray<FPlannerGoal> Goals;
	
	void UpdateStack(); 
	FPlannerAction CurrentAction;
};

int getHCost(Node A, FPlannerWorldState B)
{
	int hCost = 0;
	for (auto X : B.StateValues)
	{
		auto GoalKey = X.Key;
		bool GoalValue = X.Value;
		
		bool* CurrentValue = A.State.StateValues.Find(GoalKey);
		
		if (!CurrentValue || *CurrentValue != GoalValue)
		{
			hCost++;
		}
	}
	return hCost;
}
