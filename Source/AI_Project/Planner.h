// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Planner.generated.h"

USTRUCT(BlueprintType)
struct FPlannerWorldState
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, bool> StateValues;
	
	
	FPlannerWorldState() = default;
	FPlannerWorldState (const FPlannerWorldState &Other) = default;
	
	bool operator==(const FPlannerWorldState& Other) const
	{
		return StateValues.OrderIndependentCompareEqual(Other.StateValues);
	}
	
	bool Satisfies(const FPlannerWorldState& Other) const
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
};

UCLASS()
class AI_PROJECT_API UActionObject : public UObject
{
	GENERATED_BODY()
	UActionObject() = default;
	
public: 
	UFUNCTION(BlueprintNativeEvent)
	bool Execute();
};

USTRUCT(BlueprintType)
struct FPlannerAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name; // the name of the action, used for debugging and identification
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlannerWorldState Context; // context needed to perform action, such as target location, target actor, etc.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UActionObject* ActionObject;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlannerWorldState Effects; // the effects of the action on the world state, used for planning
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost; // the cost of performing the action, used for planning
	
	FPlannerAction(const FPlannerAction &Other) = default;
	
	FPlannerAction() = default;
	
	bool operator==(const FPlannerAction& Other) const
	{
		return Name == Other.Name; // or whatever defines equality
	}
};

USTRUCT(BlueprintType)
struct FPlannerGoal
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlannerWorldState DesiredState; // the desired world state that satisfies the goal
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority; // the priority of the goal, used for selecting between multiple goals
	
	bool operator==(const FPlannerGoal& Other) const
	{
		return Name == Other.Name;
	}
};

struct Node
{
// have an id for Node? use pointers
	FPlannerWorldState State;
	FPlannerAction Action;
	Node* Parent;
	float gCost, fCost, hCost;
	Node() : State{}, Parent(nullptr), Action{}, gCost(0), fCost(0), hCost(0) {}
	Node(FPlannerWorldState &State) : State(State) {}
	Node(FPlannerWorldState State, FPlannerAction Action, Node* Parent, float gCost, float fCost, float hCost) : State(State), Parent(Parent), Action(Action), gCost(gCost), fCost(fCost), hCost(hCost) {};
	bool operator==(const Node& Other) const
	{
		return Action.Effects == Other.Action.Effects; // compare based on the resulting world state after performing the action
	}
	
	Node(const Node &Other)
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
UINTERFACE(NotBlueprintable)
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
	TArray<FPlannerAction> FilterAvailableActions(TArray<FPlannerAction> Actions, FPlannerWorldState CurrentState);
	TArray<FPlannerAction> GetSatisfyingActions(TArray<FPlannerAction> Actions, FPlannerWorldState DesiredState);
	TArray<FPlannerAction> ToDoStack; 
	FPlannerGoal DesiredGoal; 
	TArray<FPlannerGoal> Goals;
	
	void UpdateStack(); 
	FPlannerAction CurrentAction;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlannerAction> AvailableActions; // the actions that the planner can use to achieve goals, this should be populated by the actor that implements the planner interfac
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	virtual void SetGoal(const FString& Key, const bool Value, FName GoalName) = 0;

};

inline int getHCost(Node A, FPlannerWorldState B)
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
