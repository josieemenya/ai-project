// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMComponent.h"


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
	for (auto Sub : Conditions)
	{
		UCondition* Condition = NewObject<UCondition>(this, Sub);
		if (!Condition && !Condition->Evaluate())
		{
			bCanRun = false;
			return;
		}
	}
	bCanRun = true;
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
		LastState->OnExit(Owner); 
	}
	
	
	if (CurrentState)
		CurrentState->OnEnter(Owner);
	
}

void UFSMComponent::InitStates() {
	for (auto State : States)
	{
		auto NewState = NewObject<UFSMState>(State);
		AllStates.Add(NewState);
	}
}

// Called every frame
void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CurrentState)
		return;
	
	if (CurrentState->OnRun(GetOwner()) != EExitSequenceType::RUNNING)
	{
		CurrentState->OnExit(GetOwner());
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
	for (TSubclassOf<UConsideration> Consider : Considerations)
	{
		if (Consider)
		{
			UConsideration* New = NewObject<UConsideration>(this, Consider);
			Score += New->Evaluate();
		}
	}
	return Score; 
}

EExitSequenceType UFSMState::OnEnter_Implementation(AActor* Owner) {
	return EExitSequenceType::RUNNING; 
}

EExitSequenceType UFSMState::OnExit_Implementation(AActor* Owner) {
	return EExitSequenceType::RUNNING;
}
EExitSequenceType UFSMState::OnRun_Implementation(AActor* Owner) {
	return EExitSequenceType::RUNNING;
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
	ValidStates.ValueSort([](auto State1, auto State2) { return State1 > State2; });// should sort like heap top
	
	if (!ValidStates.IsEmpty())
	{
		LastState = CurrentState;
		CurrentState = ValidStates.CreateConstIterator().Key(); 
		StartTransition.Broadcast(); 
	}
}


void UFSMComponent::UpdateAllStates(TArray<UFSMState*> All) {
	for (auto State : All) {
		for (auto Condition : State->Conditions) {
			auto NewCondition = NewObject<UCondition>(this, Condition);
			if (!NewCondition->Evaluate()) {
				State->bCanRun = false; 
			} 
		}
	}
}