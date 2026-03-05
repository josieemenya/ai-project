// Fill out your copyright notice in the Description page of Project Settings.


#include "PlannerComponent.h"
#include "GameFramework/Actor.h"
#include "ComponentUtils.h"

// Sets default values for this component's properties
UPlannerComponent::UPlannerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlannerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlannerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlannerComponent::SetGoal(TMap<FString, bool> GoalValue, FName GoalName)
{
	UPlannerGoal* NewGoal = NewObject<UPlannerGoal>();
	FWorldState GoalState =  FWorldState(); 
	GoalState.StateValues = GoalValue;
	NewGoal->Name = GoalName.ToString();
	NewGoal->DesiredState = GoalState;
	NewGoal->Priority = 1; 
	
	Goals.Add(NewGoal); 
}

void UPlannerComponent::AddToAvailableActions(UAction* NewAction)
{
		AvailableActions.Add(NewAction);
}

TArray<UAction*> UPlannerComponent::PlanGoal(FWorldState CurrentState, FWorldState DesiredState)
{
	TArray<Node*> Open;
	TArray<Node*> Close;
	
	

	auto StartNode = new Node {
		CurrentState,
		{},
		nullptr,
		0, 
		0, 
		0
	}; 
	
	auto remainingActions = getHCost(StartNode, DesiredState);
	
	StartNode->hCost = remainingActions;
	StartNode->fCost = remainingActions;

	Open.Add(StartNode);

	while (!Open.IsEmpty())
	{
		// find lowestCost
		Open.Sort([](const Node& A, const Node& B) { return A.fCost < B.fCost; });
		Node* CurrentNode = Open[0];
		Open.RemoveAt(0);
		Close.Add(CurrentNode);
		
		// check for completion
		if (CurrentNode->State.Satisfies(DesiredState))
		{
			auto Path = BuildPlan(CurrentNode);
			for (auto n : Open)
				delete n;
			for (auto n : Close) 
				delete n; 
			return Path;
		}
		
		// filter against valid actions,  check against precomditions
		auto validActions = FilterAvailableActions(AvailableActions, CurrentNode->State);
		
		// filter actions that satisfy our goal, 
		//auto satisfyingActions = GetSatisfyingActions(validActions, DesiredState);

		for (const auto &possibleAction : validActions)
		{
			//auto newWorld = new Node(UAction{"", CurrentNode.Action.Effects, []()->bool {return false; }}, nullptr, 0, 0, 0);
			Node* Child = new Node(CurrentNode->State);
			Child->Parent = CurrentNode; // a heap node pointer
			

			
			for (auto& Effect : possibleAction->Effects.StateValues)
			{
				Child->State.StateValues[Effect.Key] = Effect.Value;
			}

			Child->Action = possibleAction;
			//newWorld->Action.Context = CurrentNode->State; // same as line 58?
			Child->gCost = CurrentNode->gCost + possibleAction->Cost;
			Child->hCost = getHCost(Child, DesiredState);
			Child->fCost = Child->gCost + Child->hCost;
			Open.Add(Child); 
		}
	}

	return TArray<UAction*>();
}

TArray<UAction*> UPlannerComponent::FilterAvailableActions(TArray<UAction*> Actions, FWorldState CurrentState)
{ // positive that this isn't correct will assess tomorrow
	TArray<UAction*> ActionList;
	for (auto action : Actions)
	{
		if (CurrentState.Satisfies(action->Context)) // check if the action is valid in the current world state
		{
			ActionList.Add(action);
		}
	}
	return ActionList;
}

TArray<UAction*> UPlannerComponent::GetSatisfyingActions(TArray<UAction*> Actions, FWorldState DesiredState)
{ // positive that this isn't correct will assess tomorrow
	TArray<UAction*> ActionList;
	for (auto action : Actions)
	{
		if (action->Effects.Satisfies(DesiredState)) // check if the action's effects satisfy the desired world state
		{
			ActionList.Add(action);
		}
	}

	return ActionList;
}

void UPlannerComponent::UpdateStack()
{
	if (ToDoStack.IsEmpty())
		return;
	UE_LOG(LogTemp, Warning, TEXT("UpdateStack called"));
	CurrentAction = ToDoStack[0];
	ToDoStack.RemoveAt(0);
	FWorldState CurrentWorldState = CurrentAction->Context;
	if (CurrentAction)
	{ /*
    	auto Instance = NewObject<UActionObject>(GetOwner(), CurrentAction);
		if (Instance) 
			Instance->Execute(GetOwner());
			*/
	}

	else
	{
    	UE_LOG(LogTemp, Error, TEXT("ActionObject is null for action %s"),
        	*CurrentAction->Name.ToString());
	}
	UE_LOG(LogTemp, Warning, TEXT("Executed action"));
}

TArray<UAction*> UPlannerComponent::BuildPlan(Node* Last)
{ // this however is correct, it builds the plan by backtracking from the goal node to the start node and collecting the actions along the way
	TArray<UAction*> Plan;
	while (Last)
	{
	    Plan.Add(Last->Action);
	    Last = Last->Parent;
	}
	Algo::Reverse(Plan);  // now it’s start -> goal
		return Plan;
}


EExitSequenceType UActionObject::Execute_Implementation(AActor* Owning)
{
	// Default C++ behavior
	return EExitSequenceType::SUCCESS;
}

