#include "PlannerComponent.h"
#include "AIController.h"
#include "BlackboardSystem.h"
#include "GameFramework/Character.h"
#include "ComponentUtils.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Transform.h"
#include "Algo/Reverse.h"
#include "Editor/StatusBar/Private/SourceControlMenuHelpers.h"
#include "Engine/SceneCapture2D.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values for this component's properties
UPlannerComponent::UPlannerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPlannerComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPlannerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlannerComponent::SetGoal(TMap<FString, bool> GoalValue, FName GoalName)
{
	UPlannerGoal* NewGoal = NewObject<UPlannerGoal>();
	FWorldState GoalState; 
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
		
		UpdateSmartObjects(CurrentState);  
		
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
{
	TArray<UAction*> ActionList;
	for (auto Action : Actions)
	{
		if (UAction* A = Cast<UAction>(Action))
		{
			if (CurrentState.Satisfies(A->Context))
				ActionList.Add(A);
		}
	}
	return ActionList;
}

void UPlannerComponent::UpdateSmartObjects(FWorldState Current)
{
	AllSmartObjectsNearby.Empty();

	
	if (auto Senser = GetOwner()->FindComponentByClass<UAIPerceptionComponent>())
	{
		Senser->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), AllSmartObjectsNearby); 
	}
	
	for (ASmartObject* SmartObj : LastSmartObjectsNearby)
	{
		Current.StateValues.Remove(SmartObj->ObjectName.ToString());
	}
	
	LastSmartObjectsNearby.Empty();
	
	for (auto SObj : AllSmartObjectsNearby)
	{
		if (ASmartObject* SmartObj = Cast<ASmartObject>(SObj))
		{
			SmartObj->WriteToWorldState(Current);
			LastSmartObjectsNearby.Add(SmartObj);
		}
	}
	
}


void UPlannerComponent::UpdateStack(AActor* Owner)
{
	if (ToDoStack.IsEmpty())
		return;
	UE_LOG(LogTemp, Warning, TEXT("UpdateStack called"));
	CurrentAction = ToDoStack[0];
	ToDoStack.RemoveAt(0);
	FWorldState CurrentWorldState = CurrentAction->Context;
	if (CurrentAction)
	{ 
		CurrentAction->Execute(Owner);
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("ActionObject is null for action %s"),
			*CurrentAction->Name.ToString());
	}
	UE_LOG(LogTemp, Warning, TEXT("Executed action"));
}



TArray<UAction*> UPlannerComponent::BuildPlan(Node* Last)
{
	TArray<UAction*> Plan;
	while (Last)
	{
		Plan.Add(Last->Action);
		Last = Last->Parent;
	}
	Algo::Reverse(Plan);
	return Plan;
}

