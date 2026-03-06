// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Planner.h"
#include "AbilitySystemComponent.h"
#include "PlannerComponent.generated.h"



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


////////////////////////////////////////////////////
///
///

UENUM(BlueprintType)
enum class EValueType : uint8
{
	Int,
	Float,
	Bool,
	Vector,
	Actor
};

USTRUCT(BlueprintType)
struct FTaggedValue
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EValueType Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 intVal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float floatVal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool boolVal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector vecVal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorVal;
}; 

UCLASS(Blueprintable, BlueprintType)
class AI_PROJECT_API UActionObject : public UDataAsset
{
	GENERATED_BODY()
public:
	UActionObject() = default;
	
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTaggedValue ActionValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Owner; 
	
	UFUNCTION(BlueprintCallable)
	virtual bool Execute(AActor* OwningActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWorld* World;
	
};

UCLASS(Blueprintable, BlueprintType)
class AI_PROJECT_API UMoveActionObject : public UActionObject
{
	GENERATED_BODY()
	UMoveActionObject() = default; 
	
public:
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Execute(AActor* OwningActor) override;
};

//////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FPlannerAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name; // the name of the action, used for debugging and identification
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlannerWorldState Context; // context needed to perform action, such as target location, target actor, etc.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> ActionObject;
	
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
////////////

UCLASS()
class AI_PROJECT_API UPlannerGoal : public UObject
{
public : 
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlannerWorldState DesiredState; // the desired world state that satisfies the goal
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority; // the priority of the goal, used for selecting between multiple goals
	
	bool operator==(const UPlannerGoal& Other) const
	{
		return Name == Other.Name;
	}
};
///////
///

struct Node
{
// have an id for Node? use pointers
	FPlannerWorldState State;
	FPlannerAction Action;
	Node* Parent;
	float gCost, fCost, hCost;
	Node() : State{}, Parent(nullptr), Action{}, gCost(0), fCost(0), hCost(0) {}
	Node(FPlannerWorldState &State) : State(State) {}
	Node(FPlannerWorldState State, FPlannerAction Action, Node* Parent, float gCost, float fCost, float hCost) : State(State), Action(Action), Parent(Parent), gCost(gCost), fCost(fCost), hCost(hCost) {};
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


///////
///
///
inline int getHCost(Node* A, FPlannerWorldState B)
{
	int hCost = 0;
	for (auto X : B.StateValues)
	{
		auto GoalKey = X.Key;
		bool GoalValue = X.Value;
		
		bool* CurrentValue = A->State.StateValues.Find(GoalKey);
		
		if (!CurrentValue || *CurrentValue != GoalValue)
		{
			hCost++;
		}
	}
	return hCost;
}

////



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UPlannerComponent : public UActorComponent, public IPlanner
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlannerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBlackboardSystem* AIbBlackboardSystem; 
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	void AddToAvailableActions(FPlannerAction NewAction); 
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	TArray<FPlannerAction> PlanGoal(FPlannerWorldState CurrentState, FPlannerWorldState DesiredState); // keep in planner

	TArray<FPlannerAction> BuildPlan(Node* Last); // keep in planner
	TArray<FPlannerAction> FilterAvailableActions(TArray<FPlannerAction> Actions, FPlannerWorldState CurrentState); // keep in planner
	TArray<FPlannerAction> GetSatisfyingActions(TArray<FPlannerAction> Actions, FPlannerWorldState DesiredState); // keep in planner

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UObject*> ToDoStack;  

	UPlannerGoal* DesiredGoal; 
	TArray<UPlannerGoal*> Goals;
	
	UFUNCTION(BlueprintCallable)
	void UpdateStack(AActor* OwningActor);

	
	FPlannerAction CurrentAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlannerAction> AvailableActions; // the actions that the planner can use to achieve goals, this should be populated by the actor that implements the planner interfac
	
	
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	void SetGoal(TMap<FString, bool> GoalVal, FName GoalName);
	
	TArray<ASmartObject*> FilterActionFromSmartObject(TArray<ASmartObject*> SmartObjects, FWorldState CurrentState);
		
};



