// Fill out your copyright notice in the Description page of Project Settings.


#include "Planner.h"

#include <corecrt_io.h>

// Add default functionality here for any IPlanner functions that are not pure virtual.
TArray<FPlannerAction> IPlanner::PlanGoal(FPlannerWorldState CurrentState, FPlannerWorldState DesiredState)
{
	TArray<Node> Open;
	TArray<Node> Close;
	
	

	auto CurrentNode = Node {
		CurrentState,
		{},
		nullptr,
		0, 
		0, 
		0
	}; 
	
	auto remainingActions = getHCost(CurrentNode, DesiredState);
	
	CurrentNode.hCost = remainingActions;
	CurrentNode.fCost = remainingActions + CurrentNode.gCost;

	Open.Add(CurrentNode);

	while (!Open.IsEmpty())
	{
		// find lowestCost
		Open.Sort([](const Node& A, const Node& B) { return A.fCost < B.fCost; });
		CurrentNode = Open[0];
		Open.RemoveAt(0);
		Close.Add(CurrentNode);
		
		// check for completion
		if (CurrentNode.Action.Effects.Satisfies(DesiredState))
		{
			auto Path = BuildPlan(&CurrentNode);
			//delete Open;
			//delete Close;
			return Path;
		}
		
		// filter against valid actions,  check against precomditions
		auto validActions = FilterAvailableActions(AvailableActions, CurrentNode.Action.Context);
		
		// filter actions that satisfy our goal, 
		auto satisfyingActions = GetSatisfyingActions(validActions, DesiredState);

		for (auto possibleAction : satisfyingActions)
		{
			//auto newWorld = new Node(FPlannerAction{"", CurrentNode.Action.Effects, []()->bool {return false; }}, nullptr, 0, 0, 0);
			Node* newWorld = new Node(CurrentNode.State);
			newWorld->Parent = &CurrentNode; // a heap node pointer
			

			
			for (auto& Effect : possibleAction.Effects.StateValues)
			{
				newWorld->State.StateValues[Effect.Key] = Effect.Value;
			}

			newWorld->Action = possibleAction;
			newWorld->Action.Context = CurrentNode.State; 
			newWorld->gCost = CurrentNode.gCost + possibleAction.Cost;
			newWorld->hCost = getHCost(*newWorld, DesiredState);
			newWorld->fCost = newWorld->gCost + newWorld->hCost;
			;
			Open.Add(*newWorld); 
		}
	}

	return TArray<FPlannerAction>();
}

TArray<FPlannerAction> IPlanner::FilterAvailableActions(TArray<FPlannerAction> Actions, FPlannerWorldState CurrentState)
{ // positive that this isn't correct will assess tomorrow
	TArray<FPlannerAction> ActionList;
	for (auto action : Actions)
	{
		if (CurrentState.Satisfies(action.Context)) // check if the action is valid in the current world state
		{
			ActionList.Add(action);
		}
	}
 	return ActionList;
}


TArray<FPlannerAction> IPlanner::GetSatisfyingActions(TArray<FPlannerAction> Actions, FPlannerWorldState DesiredState)
{ // positive that this isn't correct will assess tomorrow
	TArray<FPlannerAction> ActionList;
	for (auto action : Actions)
	{
		if (action.Effects.Satisfies(DesiredState)) // check if the action's effects satisfy the desired world state
		{
			ActionList.Add(action);
		}
	}

	return ActionList;
}

void IPlanner::UpdateStack()
{
	if (ToDoStack.IsEmpty())
		return;
	
	CurrentAction = ToDoStack.Pop();
	FPlannerWorldState CurrentWorldState = CurrentAction.Context;
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
