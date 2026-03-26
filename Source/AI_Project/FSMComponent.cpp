// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMComponent.h"

#include "AI_Project.h"


UUtilityTree* UUtilityTree::Instance = nullptr;

UUtilityTree* UUtilityTree::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UUtilityTree>();

		// Prevent garbage collection
		Instance->AddToRoot();
	}

	return Instance;
}

void UUtilityTree::ScoreStates(TMap<UFSMState*, float>& States)
{
	for (auto& State : States)
	{
		State.Value = State.Key->Evaluate(); 
	}
}

void UFSMState::EvaluateConditions()
{
	bCanRun = true;

	for (auto Sub : InstancedConditions)
	{
		if (!Sub || !Sub->Evaluate())
		{
			bCanRun = false;
			return;
		}
	}
}

// Sets default values for this component's properties
UFSMComponent::UFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();
	InitStates(); 
	StartTransition.AddUObject(this, &UFSMComponent::SwitchAndRun); 
}

void UFSMComponent::SwitchAndRun()
{
	auto Owner = GetOwner();
	if (LastState && LastState != CurrentState)
	{
		if (LastState->OnExit(Owner) == EExitSequenceType::DEFAULT)
		{
			UE_LOG(
			LogTemp, 
			Error, 
			TEXT("%s's OnExit function has not been overridden/or there is a stray exeution pin that has not returned an ExitSequenceType!", LastState ? *LastState->StateName : TEXT("Unknown State"))
			); 
		} 
	}
	
	
	if (CurrentState){
		if (CurrentState->OnEnter(Owner) == EExitSequenceType::DEFAULT)
		{
			UE_LOG(
			LogTemp, 
			Error, 
			TEXT("%s's OnEnter function has not been overridden/or there is a stray exeution pin that has not returned an ExitSequenceType!", CurrentState ? *CurrentState->StateName : TEXT("Unknown State"))
			); 
		}
	}
}

void UFSMComponent::InitStates() {
	for (auto State : States)
	{
		auto NewState = NewObject<UFSMState>(this, State);
		NewState->InstanceStates(); 
		AllStates.Add(NewState);
	}
}

// Called every frame
void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CurrentState)
	{
		UpdateCurrentState();
		return;
	}
	
	if (CurrentState->OnRun(GetOwner()) == EExitSequenceType::DEFAULT)
	{
		UE_LOG(
			LogTemp, 
			Error, 
			TEXT("%s's OnRun function has not been overridden/or there is a stray exeution pin that has not returned an ExitSequenceType!", CurrentState ? *CurrentState->StateName : TEXT("Unknown State"))
			); 
	}

	if (CurrentState->OnRun(GetOwner()) != EExitSequenceType::RUNNING)
	{
		UpdateCurrentState();
	}
}

void UFSMState::InstanceStates()
{
	for (auto Sub : Conditions)
	{
		InstancedConditions.Add(NewObject<UCondition>(this, Sub));
	}

	for (auto Consider : Considerations)
	{
		InstancedConsiderations.Add(NewObject<UConsideration>(this, Consider));
	}
}

float UConsideration::Evaluate_Implementation()
{
	return 0.0f; 
}

UFSMState::UFSMState() {
}

float UFSMState::Evaluate()
{
	float Score = 0.0f;

	for (auto Consider : InstancedConsiderations)
	{
		if (Consider)
		{
			Score += Consider->Evaluate();
		}
	}
	

	return Score;
}
EExitSequenceType UFSMState::OnEnter_Implementation(AActor* Owner) {
	return EExitSequenceType::DEFAULT; 
}

EExitSequenceType UFSMState::OnExit_Implementation(AActor* Owner) {
	return EExitSequenceType::DEFAULT;
}
EExitSequenceType UFSMState::OnRun_Implementation(AActor* Owner) {
	return EExitSequenceType::DEFAULT;
}

TMap<UFSMState*, float> UFSMComponent::FilterAvailableStates(TArray<UFSMState*> AvailableStates)
{
	TMap<UFSMState*, float> validStates;
	

	UpdateAllStates(AvailableStates);

	for (auto State : AvailableStates)
	{
		if (State->bCanRun)
		{
			validStates.Add(State, 0);
		}
	}
	return validStates;
}

void UFSMComponent::UpdateCurrentState() {
	auto ValidStates = FilterAvailableStates(AllStates);
	UUtilityTree::Get()->ScoreStates(ValidStates);
	
	UFSMState* BestState = GetBestState(ValidStates);
	if (BestState && BestState != CurrentState)
	{
		LastState = CurrentState;
		CurrentState = BestState; 
		StartTransition.Broadcast(); 
	}
}

void UFSMComponent::UpdateAllStates(TArray<UFSMState*> All)
{
	for (auto State : All)
	{
		State->EvaluateConditions();
	}
}

UFSMState* UFSMComponent::GetBestState(TMap<UFSMState*, float> &All)
{
	UFSMState* BestState = nullptr;
	float BestScore = -FLT_MAX;

	for (auto Pair : All)
	{
		if (Pair.Value > BestScore)
		{
			BestScore = Pair.Value;
			BestState = Pair.Key;
		}
	}
	
	return BestState;
}

