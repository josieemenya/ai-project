// Fill out your copyright notice in the Description page of Project Settings.


#include "Planner.h"

#include <corecrt_io.h>

// Add default functionality here for any IPlanner functions that are not pure virtual.
TArray<FPlannerAction> IPlanner::PlanGoal(FPlannerWorldState* CurrentState, FPlannerWorldState* DesiredState)
{
	TArray<Node> Open;
	TArray<Node> Close;

	auto Action = new FPlannerAction();
	Action->Context = CurrentState;

	auto CurrentNode = Node{*Action, nullptr, 0, 0, 0};

	Open.Add(CurrentNode);

	while (!Open.IsEmpty())
	{
		Open.Sort([](const Node& A, const Node& B) { return A.fCost < B.fCost; });
		CurrentNode = Open[0];
		if (CurrentNode.Action.Effects == DesiredState)
		{
			auto Path = BuildPlan(&CurrentNode);
			//delete Open;
			//delete Close;
			return Path;
		}

		Open.Remove(CurrentNode);
		Close.Add(CurrentNode);

		auto validActions = FilterAvailableActions(AvailableActions, CurrentNode.Action.Context);
		auto satisfyingActions = GetSatisfyingActions(validActions, DesiredState);

		for (auto possibleAction : satisfyingActions)
		{
			auto newWorld = new Node(FPlannerAction{"", CurrentNode.Action.Effects, []()->bool {return false; }}, nullptr, 0, 0, 0);
			for (auto& Effect : possibleAction.Effects->StateValues)
			{
				(*newWorld).Action.Effects->StateValues.Add(Effect.Key, Effect.Value);
			}

			newWorld->Parent->Action = possibleAction;
			newWorld->Parent->Action.Context = CurrentNode.Action.Context;
			
			newWorld->gCost = CurrentNode.gCost + possibleAction.Cost;
			Open.Add(*newWorld); 
		}
	}

	return TArray<FPlannerAction>();
}

TArray<FPlannerAction> IPlanner::FilterAvailableActions(TArray<FPlannerAction> Actions, FPlannerWorldState* CurrentState)
{ // positive that this isn't correct will assess tomorrow
	for (auto action : Actions)
	{
		if ( action.Context == CurrentState) // check if the action is valid in the current world state
		{
			Actions.Remove(action);
		}
	}
 	return Actions;
}


TArray<FPlannerAction> IPlanner::GetSatisfyingActions(TArray<FPlannerAction> Actions, FPlannerWorldState* DesiredState)
{ // positive that this isn't correct will assess tomorrow
	for (auto action : Actions)
	{
		if (action.Effects == DesiredState) // check if the action's effects satisfy the desired world state
		{
			Actions.Remove(action);
		}
	}

	return Actions;
}

void IPlanner::UpdateStack()
{
	if (ToDoStack.IsEmpty())
		return;
	
	CurrentAction = ToDoStack.Pop();
	FPlannerWorldState* CurrentWorldState = CurrentAction.Context;
	CurrentAction.DoAction(); 
}

TArray<FPlannerAction> IPlanner::BuildPlan(Node* Last)
{ // this however is correct, it builds the plan by backtracking from the goal node to the start node and collecting the actions along the way
	TArray<FPlannerAction> Plan;
	while (Last->Parent)
	{
		Plan.Insert(Last->Action, 0);
		Last = Last->Parent;
	}

	return Plan;
}
