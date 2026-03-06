// Fill out your copyright notice in the Description page of Project Settings.


#include "PlannerComponent.h"

<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "BaseAI.h"
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
#include "AIController.h"
#include "AudioMixerBlueprintLibrary.h"
#include "BlackboardSystem.h"
#include "GameFramework/Character.h"
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
#include "ComponentUtils.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

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
	FPlannerWorldState GoalState; 
	GoalState.StateValues = GoalValue;
	NewGoal->Name = GoalName.ToString();
	NewGoal->DesiredState = GoalState;
	NewGoal->Priority = 1; 
	
	Goals.Add(NewGoal); 
}

<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
void UPlannerComponent::AddToAvailableActions(FPlannerAction NewAction)
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
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
		
		if (!DesiredActions.IsEmpty()){
			DesiredActions.Sort([](const UAction& A, const UAction& B) { return A.Cost < B.Cost; });
			obj->DesiredActionIndex = DesiredActions[0]; 
		}
	}
	
	return ObjectList;
	
}

void UPlannerComponent::AddToAvailableActions(UAction* NewAction)
>>>>>>> Stashed changes
{
		AvailableActions.Add(NewAction);
}

TArray<FPlannerAction> UPlannerComponent::PlanGoal(FPlannerWorldState CurrentState, FPlannerWorldState DesiredState)
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
		TArray<UObject*> validActions = TArray<UObject*>();
		auto ActionsArray = FilterAvailableActions(AvailableActions, CurrentNode->State);
		auto ObjectsArray = FilterActionFromSmartObject(AIbBlackboardSystem->GetValueAllSmartObjects(), CurrentNode->State);
		validActions.Reserve(ActionsArray.Num());
		Algo::Transform(ActionsArray, validActions, [](UAction* Action){ return static_cast<UObject*>(Action); });
		Algo::Transform(ObjectsArray, validActions, [](ASmartObject* Obj){ return static_cast<UObject*>(Obj); });

		
		// filter actions that satisfy our goal, 
		//auto satisfyingActions = GetSatisfyingActions(validActions, DesiredState);

		for (auto &possibleAction : validActions)
		{
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
			//auto newWorld = new Node(FPlannerAction{"", CurrentNode.Action.Effects, []()->bool {return false; }}, nullptr, 0, 0, 0);
			Node* Child = new Node(CurrentNode->State);
			Child->Parent = CurrentNode; // a heap node pointer
			

			
			for (auto& Effect : possibleAction.Effects.StateValues)
=======
			auto ObjPtr = possibleAction;
			//auto newWorld = new Node(UAction{"", CurrentNode.Action.Effects, []()->bool {return false; }}, nullptr, 0, 0, 0);
			Node* Child = new Node(CurrentNode->State);
			Child->Parent = CurrentNode; // a heap node pointer
			
			if (auto Action = Cast<UAction>(ObjPtr))
>>>>>>> Stashed changes
=======
			auto ObjPtr = possibleAction;
			//auto newWorld = new Node(UAction{"", CurrentNode.Action.Effects, []()->bool {return false; }}, nullptr, 0, 0, 0);
			Node* Child = new Node(CurrentNode->State);
			Child->Parent = CurrentNode; // a heap node pointer
			
			if (auto Action = Cast<UAction>(ObjPtr))
>>>>>>> Stashed changes
=======
			auto ObjPtr = possibleAction;
			//auto newWorld = new Node(UAction{"", CurrentNode.Action.Effects, []()->bool {return false; }}, nullptr, 0, 0, 0);
			Node* Child = new Node(CurrentNode->State);
			Child->Parent = CurrentNode; // a heap node pointer
			
			if (auto Action = Cast<UAction>(ObjPtr))
>>>>>>> Stashed changes
			{
				for (auto& Effect : Action->Effects.StateValues)
				{
					Child->State.StateValues[Effect.Key] = Effect.Value;
				}
				Child->Action = Action;
				Child->gCost = CurrentNode->gCost + Action->Cost; 
			}
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream

			Child->Action = possibleAction;
			//newWorld->Action.Context = CurrentNode->State; // same as line 58?
			Child->gCost = CurrentNode->gCost + possibleAction.Cost;
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
			
			else if (AActor* actor = Cast<ASmartObject>(ObjPtr))
			{
				if (ASmartObject* SmartObj = Cast<ASmartObject>(actor)) {
					if (SmartObj->DesiredActionIndex)
					{
						for (auto& Effect : SmartObj->DesiredActionIndex->Effects.StateValues)
						{
							Child->State.StateValues[Effect.Key] = Effect.Value;
						}
						Child->Action = SmartObj->DesiredActionIndex;
						Child->gCost = CurrentNode->gCost + SmartObj->DesiredActionIndex->Cost;
					}
					
					else
					{
						delete Child; // skip invalid smart object action
						continue;
					}
				}
			}
			
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
			Child->hCost = getHCost(Child, DesiredState);
			Child->fCost = Child->gCost + Child->hCost;
			Open.Add(Child); 
		}
	}

	return TArray<FPlannerAction>();
}

TArray<FPlannerAction> UPlannerComponent::FilterAvailableActions(TArray<FPlannerAction> Actions, FPlannerWorldState CurrentState)
{ // positive that this isn't correct will assess tomorrow
	TArray<FPlannerAction> ActionList;
	for (auto action : Actions)
	{
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
		if (CurrentState.Satisfies(action.Context)) // check if the action is valid in the current world state
=======
		if (auto A = Cast<UAction>(action))
>>>>>>> Stashed changes
=======
		if (auto A = Cast<UAction>(action))
>>>>>>> Stashed changes
=======
		if (auto A = Cast<UAction>(action))
>>>>>>> Stashed changes
		{
			if (CurrentState.Satisfies(A->Context)) // check if the action is valid in the current world state
			{
				ActionList.Add(A);
			}
		}
		
	}
	return ActionList;
}

TArray<FPlannerAction> UPlannerComponent::GetSatisfyingActions(TArray<FPlannerAction> Actions, FPlannerWorldState DesiredState)
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

<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
void UPlannerComponent::UpdateStack(AActor* OwningActor)
=======
void UPlannerComponent::UpdateStack(AActor* Owner)
>>>>>>> Stashed changes
=======
void UPlannerComponent::UpdateStack(AActor* Owner)
>>>>>>> Stashed changes
=======
void UPlannerComponent::UpdateStack(AActor* Owner)
>>>>>>> Stashed changes
{
	if (ToDoStack.IsEmpty())
		return;
	UE_LOG(LogTemp, Warning, TEXT("UpdateStack called"));
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
	CurrentAction = ToDoStack[0];
	ToDoStack.RemoveAt(0);
	FPlannerWorldState CurrentWorldState = CurrentAction.Context;
	if (CurrentAction.ActionObject)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CurrentAction.ActionObject); 
    	if (auto AI = Cast<ABaseAI>(GetOwner()))
    	{
    		if (UAbilitySystemComponent* ASC = AI->GetAbilitySystemComponent())
    		{
    			FGameplayAbilitySpec Spec(CurrentAction.ActionObject);
    			FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec);
    			ASC->TryActivateAbility(Handle);
    		}
    	}
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
	
	if (auto a  = Cast<UAction>(ToDoStack[0]))
	{
		ToDoStack.RemoveAt(0);
		CurrentAction = a; 
		FWorldState CurrentWorldState = CurrentAction->Context;
	} 
	else if (auto b  = Cast<ASmartObject>(ToDoStack[0]))
	{
		auto AIMoving = Cast<AAIController>(Cast<ACharacter>(Owner)->GetController()); 
		if (AIMoving)
		{
			ToDoStack.RemoveAt(0);
			AIMoving->MoveToActor(b); 
			CurrentAction = b->DesiredActionIndex;
			FWorldState CurrentWorldState = CurrentAction->Context;
		}
	}
	
	
	if (CurrentAction)
	{ 
		CurrentAction->Execute(Owner); 
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
	}
	else
	{
    	UE_LOG(LogTemp, Error, TEXT("ActionObject is null for action %s"),
        	*CurrentAction.Name.ToString());
	}
	UE_LOG(LogTemp, Warning, TEXT("Executed action"));
}

TArray<FPlannerAction> UPlannerComponent::BuildPlan(Node* Last)
{ // this however is correct, it builds the plan by backtracking from the goal node to the start node and collecting the actions along the way
	TArray<FPlannerAction> Plan;
	while (Last)
	{
	    Plan.Add(Last->Action);
	    Last = Last->Parent;
	}
	Algo::Reverse(Plan);  // now it’s start -> goal
		return Plan;
}


bool UActionObject::Execute(AActor* Actor)
{
	World = GetWorld(); // always get world context
	return true;
}



bool UMoveActionObject::Execute(AActor* Actor)
{
	Super::Execute(Actor); 
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owning Actor not valid!"));
		return false;
	}
	APawn* Pawn = Cast<APawn>(Actor);
	if (!Pawn)
	{
		return false;
	}
	
	AAIController* Controller = Cast<AAIController>(Pawn->Controller);
	
	if (!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller not valid"));
		return false;
	}
	
	World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World not found!"));
		return false;
	}
	
	
	switch(ActionValue.Type)
	{
		case EValueType::Actor:
			{
				TArray<AActor*> ActorsInWorld;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActionValue.ActorVal, ActorsInWorld); 
				
				if (ActorsInWorld.Num() == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("No actors of that class found"));
					break;
				}
				
				ActorsInWorld.Sort([Actor](auto& A, auto& B)
					{
						return FVector::Dist(A.GetActorLocation(), Actor->GetActorLocation()) <
							FVector::Dist(B.GetActorLocation(), Actor->GetActorLocation());
					}	
				); 
				Controller->MoveToActor(ActorsInWorld[0], 50.f); 
				break;
			}
		
		case EValueType::Vector:
			Controller->MoveToLocation(ActionValue.vecVal, 50.f);
			break;
	}
	
	return true;
}