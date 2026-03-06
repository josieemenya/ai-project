#include "PlannerComponent.h"

#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "BaseAI.h"
#include "BlackboardSystem.h"
#include "GameFramework/Character.h"
#include "ComponentUtils.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Transform.h"
#include "Algo/Reverse.h"

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
	FPlannerWorldState GoalState; 
	GoalState.StateValues = GoalValue;

	NewGoal->Name = GoalName.ToString();
	NewGoal->DesiredState = GoalState;
	NewGoal->Priority = 1; 
	
	Goals.Add(NewGoal); 
}

TArray<ASmartObject*> UPlannerComponent::FilterActionFromSmartObject(TArray<ASmartObject*> SmartObjects, FWorldState CurrentState)
{
	TArray<ASmartObject*> ObjectList; 
	
	for (auto obj : SmartObjects)
	{
		TArray<UAction*> DesiredActions;
		for (auto Action : obj->PossibleActions)
		{
			if (CurrentState.Satisfies(Action->Context))
			{
				DesiredActions.Add(Action);
				ObjectList.Add(obj);
			}
		}
		
		if (!DesiredActions.IsEmpty())
		{
			DesiredActions.Sort([](const UAction& A, const UAction& B) { return A.Cost < B.Cost; });
			obj->DesiredActionIndex = DesiredActions[0]; 
		}
	}
	
	return ObjectList;
}

void UPlannerComponent::AddToAvailableActions(UAction* NewAction)
{
	AvailableActions.Add(NewAction);
}

TArray<FPlannerAction> UPlannerComponent::PlanGoal(FPlannerWorldState CurrentState, FPlannerWorldState DesiredState)
{
	TArray<Node*> Open;
	TArray<Node*> Close;

	Node* StartNode = new Node{ CurrentState, {}, nullptr, 0, 0, 0 }; 
	StartNode->hCost = getHCost(StartNode, DesiredState);
	StartNode->fCost = StartNode->hCost;

	Open.Add(StartNode);

	while (!Open.IsEmpty())
	{
		// Find node with lowest fCost
		Open.Sort([](const Node& A, const Node& B) { return A.fCost < B.fCost; });
		Node* CurrentNode = Open[0];
		Open.RemoveAt(0);
		Close.Add(CurrentNode);

		// Check if goal reached
		if (CurrentNode->State.Satisfies(DesiredState))
		{
			TArray<FPlannerAction> Path = BuildPlan(CurrentNode);
			for (auto n : Open) delete n;
			for (auto n : Close) delete n;
			return Path;
		}

		// Filter valid actions
		TArray<UObject*> ValidActions;
		TArray<FPlannerAction> ActionsArray = FilterAvailableActions(AvailableActions, CurrentNode->State);
		TArray<ASmartObject*> ObjectsArray = FilterActionFromSmartObject(AIbBlackboardSystem->GetValueAllSmartObjects(), CurrentNode->State);

		ValidActions.Reserve(ActionsArray.Num() + ObjectsArray.Num());
		Algo::Transform(ActionsArray, ValidActions, [](UAction* Action){ return static_cast<UObject*>(Action); });
		Algo::Transform(ObjectsArray, ValidActions, [](ASmartObject* Obj){ return static_cast<UObject*>(Obj); });

		for (auto& PossibleAction : ValidActions)
		{
			Node* Child = new Node(CurrentNode->State);
			Child->Parent = CurrentNode;

			if (UAction* Action = Cast<UAction>(PossibleAction))
			{
				for (auto& Effect : Action->Effects.StateValues)
					Child->State.StateValues[Effect.Key] = Effect.Value;

				Child->Action = Action;
				Child->gCost = CurrentNode->gCost + Action->Cost;
			}
			else if (ASmartObject* SmartObj = Cast<ASmartObject>(PossibleAction))
			{
				if (SmartObj->DesiredActionIndex)
				{
					for (auto& Effect : SmartObj->DesiredActionIndex->Effects.StateValues)
						Child->State.StateValues[Effect.Key] = Effect.Value;

					Child->Action = SmartObj->DesiredActionIndex;
					Child->gCost = CurrentNode->gCost + SmartObj->DesiredActionIndex->Cost;
				}
				else
				{
					delete Child;
					continue;
				}
			}

			Child->hCost = getHCost(Child, DesiredState);
			Child->fCost = Child->gCost + Child->hCost;
			Open.Add(Child); 
		}
	}

	return {};
}

TArray<FPlannerAction> UPlannerComponent::FilterAvailableActions(TArray<FPlannerAction> Actions, FPlannerWorldState CurrentState)
{
	TArray<FPlannerAction> ActionList;
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

TArray<FPlannerAction> UPlannerComponent::GetSatisfyingActions(TArray<FPlannerAction> Actions, FPlannerWorldState DesiredState)
{
	TArray<FPlannerAction> ActionList;
	for (auto Action : Actions)
	{
		if (Action.Effects.Satisfies(DesiredState))
			ActionList.Add(Action);
	}
	return ActionList;
}

void UPlannerComponent::UpdateStack(AActor* Owner)
{
	if (ToDoStack.IsEmpty()) return;

	UObject* CurrentObject = ToDoStack[0];
	ToDoStack.RemoveAt(0);

	if (UAction* Action = Cast<UAction>(CurrentObject))
	{
		CurrentAction = Action;
		FWorldState CurrentWorldState = CurrentAction->Context;
		CurrentAction->Execute(Owner);
	}
	else if (ASmartObject* SmartObj = Cast<ASmartObject>(CurrentObject))
	{
		if (AAIController* AIMoving = Cast<AAIController>(Cast<ACharacter>(Owner)->GetController()))
		{
			AIMoving->MoveToActor(SmartObj);
			CurrentAction = SmartObj->DesiredActionIndex;
			FWorldState CurrentWorldState = CurrentAction->Context;
			CurrentAction->Execute(Owner);
		}
	}
}

TArray<FPlannerAction> UPlannerComponent::BuildPlan(Node* Last)
{
	TArray<FPlannerAction> Plan;
	while (Last)
	{
		Plan.Add(Last->Action);
		Last = Last->Parent;
	}
	Algo::Reverse(Plan);
	return Plan;
}

//---------------- Action Objects ----------------//

bool UActionObject::Execute(AActor* Actor)
{
	World = GetWorld();
	return true;
}

bool UMoveActionObject::Execute(AActor* Actor)
{
	Super::Execute(Actor);
	if (!Actor) return false;

	APawn* Pawn = Cast<APawn>(Actor);
	if (!Pawn) return false;

	AAIController* Controller = Cast<AAIController>(Pawn->Controller);
	if (!Controller) return false;

	World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return false;

	switch(ActionValue.Type)
	{
		case EValueType::Actor:
		{
			TArray<AActor*> ActorsInWorld;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActionValue.ActorVal, ActorsInWorld); 

			if (ActorsInWorld.Num() == 0) break;

			ActorsInWorld.Sort([Actor](AActor& A, AActor& B)
			{
				return FVector::Dist(A.GetActorLocation(), Actor->GetActorLocation()) <
					   FVector::Dist(B.GetActorLocation(), Actor->GetActorLocation());
			});

			Controller->MoveToActor(ActorsInWorld[0], 50.f); 
			break;
		}
		case EValueType::Vector:
			Controller->MoveToLocation(ActionValue.vecVal, 50.f);
			break;
	}

	return true;
}