// Fill out your copyright notice in the Description page of Project Settings.


#include "testGOAP.h"

#include "Engine/Engine.h"

testGOAP::testGOAP()
{
}

testGOAP::~testGOAP()
{
}

TArray<FGOAPAction*> testGOAP::Plan(TArray<FGOAPAction*> AvailableActions, FWorldState* CurrentState, FGOAPGoal* Goal)
{
	TArray<FGOAPAction*> usableActions = TArray<FGOAPAction*>();

	for (auto A : AvailableActions )
	{
		if (A->isAchievable())
		{
			usableActions.Add(A);
		}
	}

	TArray<Node*> leaves = TArray<Node*>();
	auto start = new Node();
	start->Parent = nullptr;
	start->Cost = 0;
	start->State = CurrentState; // check this again, it might be an array that came from single ton??
	start->Action = nullptr;
	
	bool bSuccess = BuildGraph(start, leaves, usableActions, Goal);
	if (!bSuccess)
	{
		// No plan found
		return TArray<FGOAPAction*>();
	}

	Node* Cheapest = nullptr;
	for (auto Leaf : leaves)
	{
		if (!Cheapest || Leaf->Cost < Cheapest->Cost)
			Cheapest = Leaf;
	}

	TArray<FGOAPAction*> BestPlan = TArray<FGOAPAction*>();
	auto N = Cheapest;

	while (N) {
		if (N->Action)
		{
			BestPlan.Insert(N->Action, 0);
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Action: %s"), *N->Action->Name));
		}
		N = N->Parent;
	}

	return BestPlan;
}

bool testGOAP::BuildGraph(Node* Parent, TArray<Node*>& Leaves, TArray<FGOAPAction*> UsableActions, FGOAPGoal* Goal)
{
	bool bFoundOne = false;

	for (auto Action : UsableActions)
	{
		if (Action->isAchievableGiven(Parent->State))
		{
			FWorldState* NewState = new FWorldState(*Parent->State);
			
			for (auto Effect : Action->Effects)
			{
				if (!NewState->StateValues.Contains(Effect.Key))
					NewState->StateValues.Add(Effect.Key, Effect.Value);
			}

			Node* NewNode = new Node();
			NewNode->Parent = Parent;
			NewNode->Cost = Parent->Cost + Action->Cost;
			NewNode->State = NewState;
			NewNode->Action = Action;

			if (Goal->isSatisfiedBy(NewState))
			{
				Leaves.Add(NewNode);
				bFoundOne = true;
			}
			else
			{
				TArray<FGOAPAction*> SubSet = TArray<FGOAPAction*>();
				for (auto A : UsableActions)
				{
					if (A != Action)
						SubSet.Add(A);
				}

				bool bFound = BuildGraph(NewNode, Leaves, SubSet, Goal);
				if (bFound)
					bFoundOne = true;
			}
		}
	}

	return bFoundOne;
}