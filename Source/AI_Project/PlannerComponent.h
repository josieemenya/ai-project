// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartObject.h"
#include "Components/ActorComponent.h"
#include "Planner.h"
#include "PlannerComponent.generated.h"

class ASmartObject; 


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
class AI_PROJECT_API UGoal : public UDataAsset
{
public : 
	// add contecxt values for goal
	// put comnditions on the goals
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRequiresSmartObject; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWorldState DesiredState; // the desired world state that satisfies the goal
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority; // the priority of the goal, used for selecting between multiple goals
	
	bool operator==(const UGoal& Other) const
	{
		return Name == Other.Name;
	}
};
///////
///

struct Node
{
	// have an id for Node? use pointers
	FWorldState State;
	UAction* Action;
	Node* Parent;
	float gCost, fCost, hCost;
	Node() : State{}, Parent(nullptr), Action{}, gCost(0), fCost(0), hCost(0) {}
	Node(FWorldState &State) : State(State) {}
	Node(FWorldState State, UAction* Action, Node* Parent, float gCost, float fCost, float hCost) : State(State), Action(Action), Parent(Parent), gCost(gCost), fCost(fCost), hCost(hCost) {};
	bool operator==(const Node& Other) const
	{
		return Action->Effects == Other.Action->Effects; // compare based on the resulting world state after performing the action
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
inline int getHCost(Node* A, FWorldState B)
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
	class UBlackboardComponent* BB_Planner;
	
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	void AddToAvailableActions(UAction* NewAction); 
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	TArray<UAction*> PlanGoal(FWorldState& CurrentState, FWorldState DesiredState); // keep in planner

	TArray<UAction*> BuildPlan(Node* Last); // keep in planner
	TArray<UAction*> FilterAvailableActions(TArray<TSubclassOf<UAction>> Actions, FWorldState CurrentState); // keep in planner
	
	UFUNCTION(BlueprintCallable)
	void UpdateSmartObjects(FWorldState& Current);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AActor*> AllSmartObjectsNearby; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USmartObjectContainer> LastSmartObjectContainer; 

	
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAction*> ToDoStack;  
	
	UGoal* DesiredGoal; 
	TArray<TSubclassOf<UGoal>> Goals;
	
	UFUNCTION(BlueprintCallable)
	void UpdateStack(AActor* OwningActor);
	
	DECLARE_MULTICAST_DELEGATE(FOnPlanInvalid);
	FOnPlanInvalid OnPlanInvalid;
	
	UAction* CurrentAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UAction>> AvailableActions; // the actions that the planner can use to achieve goals, this should be populated by the actor that implements the planner interfac
	
	
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	void SetGoal(TSubclassOf<UGoal> GoalClass);
	
};