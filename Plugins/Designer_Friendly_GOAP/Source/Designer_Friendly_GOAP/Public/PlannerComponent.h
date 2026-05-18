// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartObject.h"
#include "Components/ActorComponent.h"
#include "Engine/DeveloperSettings.h"
#include "PlannerComponent.generated.h"

class UBlackboardComponent;


////////////////////////////////////////////////////
///
///
///
///

UCLASS(Config=Game, DefaultConfig)
class DESIGNER_FRIENDLY_GOAP_API UPlannerLoggerSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, Config, Category="Planner Log")
	bool bShouldShowLogs; 
};

UCLASS()
class DESIGNER_FRIENDLY_GOAP_API UPlannerLogger : public UWorldSubsystem
{
	GENERATED_BODY()
	
	bool ShouldShowLogs; 
public:
	void Log(const FString& Message);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};

UCLASS(Blueprintable, BlueprintType,  meta=(ShowWorldContextPin))
class DESIGNER_FRIENDLY_GOAP_API UGoal : public UDataAsset
{
public : 
	// add contecxt values for goal
	// put comnditions on the goals
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal")
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal")
	bool bRequiresSmartObject; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal")
	FWorldState DesiredState; // the desired world state that satisfies the goal
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal")
	int32 Priority; // the priority of the goal, used for selecting between multiple goals
	
	UFUNCTION(BlueprintNativeEvent, Category="Goal")
	float GetUtility(const UBlackboardComponent* BlackBoard); // needs BBlackboard
	
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



UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESIGNER_FRIENDLY_GOAP_API UPlannerComponent : public UActorComponent
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
	
	UBlackboardComponent* BB_Planner;
	
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	void AddToAvailableActions(UAction* NewAction);

	TArray<UAction*> FilterSatisfyingActions(TArray<UAction*> Array, const FWorldState& State);
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	TArray<UAction*> PlanGoal(FWorldState& CurrentState, FWorldState DesiredState); // keep in planner

	TArray<UAction*> BuildPlan(Node* Last); // keep in planner
	TArray<UAction*> FilterAvailableActions(TArray<UAction*> Actions, FWorldState CurrentState); // keep in planner
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	void UpdateSmartObjects(FWorldState& Current);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Planner")
	TArray<AActor*> AllSmartObjectsNearby; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Planner")
	TObjectPtr<USmartObjectsContainer> LastSmartObjectContainer; 

	
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Planner")
	TArray<UAction*> ToDoStack;  
	
	UGoal* DesiredGoal; 
	TArray<TSubclassOf<UGoal>> Goals;
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	void UpdateStack(AActor* OwningActor);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlanInvalid);
	FOnPlanInvalid OnPlanInvalid;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlanFinished); 
	FOnPlanFinished OnPlanFinished;
	
	UAction* CurrentAction;
	UAction* LastAction; // for debugging purposes only
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Planner")
	TArray<TSubclassOf<UAction>> AvailableActions; // the actions that the planner can use to achieve goals, this should be populated by the actor that implements the planner interfac

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Planner")
	TArray<UAction*> ActionList;
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	void SetGoal(TSubclassOf<UGoal> GoalClass);
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	FORCEINLINE bool HasPlan() const { return ToDoStack.Num() > 0; } 
	
	UFUNCTION(BlueprintCallable, Category="Planner")
	FORCEINLINE void AbortPlan() { ToDoStack.Empty(); }

	FWorldState AgentStateValue; 
};