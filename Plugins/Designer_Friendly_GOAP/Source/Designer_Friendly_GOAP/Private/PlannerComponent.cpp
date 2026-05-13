#include "PlannerComponent.h"
#include "AIController.h"
#include "GPController.h"
#include "GameFramework/Actor.h"
#include "Algo/Transform.h"
#include "Algo/Reverse.h"
#include "Animation/AnimMontage.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h" 


float UGoal::GetUtility_Implementation(const UBlackboardComponent* BlackBoard)
{
	return 0.f; 
}

// Sets default values for this component's properties
UPlannerComponent::UPlannerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UPlannerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (AAIController* AIController = Cast<AAIController>(GetOwner()))
	{
		BB_Planner = AIController->GetBlackboardComponent();
	}
	
	LastSmartObjectContainer = NewObject<USmartObjectContainer>(this);
	for (TSubclassOf<UAction> ActionClass : AvailableActions)
	{
		UAction* InstanceAction = NewObject<UAction>(this, ActionClass.Get());
		ActionList.Add(InstanceAction);
	}
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
	
	//UE_LOG(LogTemp, Warning, TEXT("Planning"));
	
	TArray<Node*> Open = TArray<Node*>();
	TArray<Node*> Close = TArray<Node*>();
	
	

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
		
		UE_LOG(LogTemp, Warning, TEXT("NODE EXPAND START"));
		
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
		//UE_LOG(LogTemp, Warning, TEXT("PlanGoal running, CurrentNode->State keys = %d"), CurrentNode->State.StateValues.Num());
		
		
		
		auto validActions = FilterAvailableActions(ActionList, CurrentNode->State);
		
		UE_LOG(LogTemp, Warning, TEXT("ValidActions = %d"), validActions.Num());
		
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

			UE_LOG(LogTemp, Warning, TEXT("Open size: %d"), Open.Num())	
			UE_LOG(LogTemp, Warning, TEXT("SOpen: %d Close: %d"), Open.Num(), Close.Num());
		
		}
	}

	return TArray<UAction*>();
}


TArray<UAction*> UPlannerComponent::FilterAvailableActions(TArray<UAction*> Actions, FWorldState CurrentState)
{
	//UE_LOG(LogTemp, Warning, TEXT("FilterAvailableActions called, AvailableActions.Num() = %d"), Actions.Num());
	
	TArray<UAction*> ResultActionList;

	for (UAction* Instance : Actions)
	{
		if (!Instance) continue;

		
		//UE_LOG(LogTemp, Warning, TEXT("Checking action %s"), *Instance->Name.ToString());
		
		if (CurrentState.Satisfies(Instance->Context))
		{
			ResultActionList.Add(Instance);
			//UE_LOG(LogTemp, Warning, TEXT("Action %s is valid"), *Instance->Name.ToString());

		} else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Action %s rejected"), *Instance->Name.ToString());
		}
			
	}

	return ResultActionList;
}

void UPlannerComponent::UpdateSmartObjects(FWorldState& Current)
{
	AllSmartObjectsNearby.Empty();
	
	auto Owner = Cast<AAIController>(GetOwner()); 
	auto AI = (Owner)? Owner->GetPawn() : Cast<APawn>(GetOwner()); // owner shouldl either be the pawn itself or the controller
	
	if (AI) {
		auto Steer =  AI->GetController(); 
		if (auto Senser = Steer->FindComponentByClass<UAIPerceptionComponent>())
		{
			Senser->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), AllSmartObjectsNearby); 
			//UE_LOG(LogTemp, Warning, TEXT("Collected All Smart Objects"));
		}	else
		{
			//UE_LOG(LogTemp, Error, TEXT("No PerceptionComponent on BaseAI"));
		}
	} else
	{
		//UE_LOG(LogTemp, Error, TEXT("Owning AI is not a pawn, or a controller"));
	}
	
	for (ASmartObject* SmartObj : LastSmartObjectContainer->RegisteredObjects)
	{
		if (SmartObj)
			Current.StateValues.Remove(SmartObj->ObjectName.ToString());
	}
	
	LastSmartObjectContainer->RegisteredObjects.Empty();
	
	for (auto SObj : AllSmartObjectsNearby)
	{
		if (ASmartObject* SmartObj = Cast<ASmartObject>(SObj))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Writing to World State"));
			SmartObj->WriteToWorldState(Current);
			
			LastSmartObjectContainer->RegisteredObjects.Add(SmartObj);
			if (!BB_Planner->GetBlackboardAsset())
			{
				//UE_LOG(LogTemp, Error, TEXT("BlackboardComponent is null"));
				return;
			}
			
			
			if (BB_Planner->GetBlackboardAsset())
			{
				BB_Planner->SetValueAsObject("Smart Objects", LastSmartObjectContainer);
			}else
			{
				//UE_LOG(LogTemp, Error, TEXT("No Value Asset for Blackboard Object"));
			}
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

	if (LastAction && CurrentAction != LastAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("Executing action: %s"), *CurrentAction->Name.ToString());
	}
	
    EExitSequenceType Result = CurrentAction->Execute(Owner);

    switch (Result)
    {
        case EExitSequenceType::RUNNING:
            return;

        case EExitSequenceType::INVALID:
            OnPlanInvalid.Broadcast();
			LastAction = CurrentAction;
            CurrentAction = nullptr;
            return;

        case EExitSequenceType::SUCCESS:
            if (auto Bot = Cast<AGPController>(GetOwner()))
            {
                for (auto& Effect : CurrentAction->Effects.StateValues)
                {
                    Bot->CurrentState.StateValues.FindOrAdd(Effect.Key) = Effect.Value;
                    /*UE_LOG(LogTemp, Warning, TEXT("Updated CurrentState: %s = %s"),
                        *Effect.Key, Effect.Value ? TEXT("true") : TEXT("false"));*/
                }
            }
    		LastAction = CurrentAction;
			ToDoStack.RemoveAt(0);
            CurrentAction = nullptr;
            break;
    	
		case EExitSequenceType::FAILURE:
    		UE_LOG(LogTemp, Error, TEXT("%s's Action execution ended in failure, please see Execute action for details."), CurrentAction ? *CurrentAction->Name.ToString() : TEXT("UnknownAction"))
    		break; 
    	
		case EExitSequenceType::DEFAULT:
    		UE_LOG(LogTemp, Error, TEXT("Hidden Enum Type reached, check %s's Execute function to see if it has not been overriden"), CurrentAction ? *CurrentAction->Name.ToString() : TEXT("UnknownAction"));
    		break;
    	
    	default:
    		UE_LOG(LogTemp, Error, TEXT("Impossible Result Type Reached. Please check %s's Execute."), CurrentAction ? *CurrentAction->Name.ToString() : TEXT("UnknownAction"));
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

