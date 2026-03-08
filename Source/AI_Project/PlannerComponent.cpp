#include "PlannerComponent.h"
#include "AIController.h"
#include "BaseAI.h"
#include "BlackboardSystem.h"
#include "GameFramework/Character.h"
#include "ComponentUtils.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Transform.h"
#include "Algo/Reverse.h"
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
	AIbBlackboardSystem = NewObject<UBlackboardSystem>(this);
	AIbBlackboardSystem->Blackboard = NewObject<UBlackboardCustom>(this);
}

// Called every frame
void UPlannerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UPlannerComponent::SetGoal(TSubclassOf<UGoal> GoalClass)
{
	Goals.Add(GoalClass); 
}


void UPlannerComponent::AddToAvailableActions(UAction* NewAction)
{
	//AvailableActions.Add(NewAction);
}

TArray<UAction*> UPlannerComponent::PlanGoal(FWorldState& CurrentState, FWorldState DesiredState)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Planning"));

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
			
			ToDoStack = Path; 
			for (auto n : Open)
				delete n;
			for (auto n : Close) 
				delete n; 
			return Path;
		}
		
		// filter against valid actions,  check against precomditions
		UE_LOG(LogTemp, Warning, TEXT("PlanGoal running, CurrentNode->State keys = %d"), CurrentNode->State.StateValues.Num());
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
				Child->State.StateValues.Add(Effect.Key, Effect.Value);
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


TArray<UAction*> UPlannerComponent::FilterAvailableActions(TArray<TSubclassOf<UAction>> Actions, FWorldState CurrentState)
{
	UE_LOG(LogTemp, Warning, TEXT("FilterAvailableActions called, AvailableActions.Num() = %d"), Actions.Num());
	
	TArray<UAction*> ActionList;

	for (TSubclassOf<UAction> ActionClass : Actions)
	{
		if (!ActionClass) continue;

		UAction* A = NewObject<UAction>(this, ActionClass.Get());
		UE_LOG(LogTemp, Warning, TEXT("Checking action %s"), *A->Name.ToString());

		for (auto& Pair : CurrentState.StateValues)
		{
			UE_LOG(LogTemp, Warning, TEXT("CurrentState: %s = %s"), *Pair.Key, Pair.Value ? TEXT("true") : TEXT("false"));
		}


		if (CurrentState.Satisfies(A->Context))
		{
			ActionList.Add(A);
			UE_LOG(LogTemp, Warning, TEXT("Action %s is valid"), *A->Name.ToString());

		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("Action %s rejected"), *A->Name.ToString());
		}
			
	}

	return ActionList;
}

void UPlannerComponent::UpdateSmartObjects(FWorldState& Current)
{
	AllSmartObjectsNearby.Empty();

	if (auto AI = Cast<APawn>(GetOwner())) {
		auto Steer =  AI->GetController(); 
		if (auto Senser = Steer->FindComponentByClass<UAIPerceptionComponent>())
		{
			Senser->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), AllSmartObjectsNearby); 
			UE_LOG(LogTemp, Warning, TEXT("Collected All Smart Objects"));
		}	else
		{
			UE_LOG(LogTemp, Error, TEXT("No PerceptionComponent on BaseAI"));
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Owning AI is not a pawn"));
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
			UE_LOG(LogTemp, Warning, TEXT("Writing to World State"));
			SmartObj->WriteToWorldState(Current);
			
			LastSmartObjectsNearby.Add(SmartObj);
			if (!AIbBlackboardSystem)
			{
				UE_LOG(LogTemp, Error, TEXT("AIbBlackboardSystem is null"));
				return;
			}

			if (!AIbBlackboardSystem->Blackboard)
			{
				UE_LOG(LogTemp, Error, TEXT("Blackboard is null"));
				return;
			}
			SmartObj->RegisterInBlackboard(AIbBlackboardSystem->Blackboard);
		}
	}
	
}


void UPlannerComponent::UpdateStack(AActor* Owner)
{
	if (ToDoStack.IsEmpty() && !CurrentAction)
	{
		OnPlanInvalid.Broadcast();
		return;
	}

	if (ToDoStack.IsEmpty())
		return;
	
	

	if (!CurrentAction && ToDoStack.Num() > 0)
	{
		CurrentAction = ToDoStack[0];
	}
	
	if (!CurrentAction)
        return; 

    UE_LOG(LogTemp, Warning, TEXT("Executing action: %s"), *CurrentAction->Name.ToString());

    EExitSequenceType Result = CurrentAction->Execute(Owner);

    switch (Result)
    {
        case EExitSequenceType::RUNNING:
            return;

        case EExitSequenceType::INVALID:
            OnPlanInvalid.Broadcast();
            CurrentAction = nullptr;
            return;

        case EExitSequenceType::SUCCESS:
            if (auto Bot = Cast<ABaseAI>(GetOwner()))
            {
                for (auto& Effect : CurrentAction->Effects.StateValues)
                {
                    Bot->CurrentState.StateValues.FindOrAdd(Effect.Key) = Effect.Value;
                    UE_LOG(LogTemp, Warning, TEXT("Updated CurrentState: %s = %s"),
                        *Effect.Key, Effect.Value ? TEXT("true") : TEXT("false"));
                }
            }
			ToDoStack.RemoveAt(0);
            CurrentAction = nullptr;
            break;
    }
}



TArray<UAction*> UPlannerComponent::BuildPlan(Node* Last)
{
	TArray<UAction*> Plan;
	while (Last)
	{
		if (Last->Action)
			Plan.Add(Last->Action);
		Last = Last->Parent;
	}
	Algo::Reverse(Plan);
	return Plan;
}

