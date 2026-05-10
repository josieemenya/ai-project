// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMComponent.h"

#include "AIController.h"
#include "AI_Project.h"
#include "HLSLTree/HLSLTreeTypes.h"
#include "Shader/ShaderTypes.h"


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
		if (State.Key)
		{
			State.Value = State.Key->Evaluate();
		}
	}
}

void UFSMState::EvaluateConditions()
{
	bCanRun = true;

	for (auto Sub : InstancedConditions)
	{
		APawn* OwnerA = Cast<APawn>(GetOuter()); // most like 
		APawn* PassedInActor = nullptr; 
		if (OwnerA) // most like a controller but in the event i actually did attach to an actor 
		{
			PassedInActor = OwnerA; 
		} else 
		{
			auto OwnerB = Cast<AAIController>(GetOuter());
			
			if (OwnerB)
				PassedInActor = Cast<APawn>(OwnerB->GetOwner());
		}
		if (!Sub || !Sub->Evaluate(PassedInActor))
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
			TEXT("%s's OnExit function has not been overridden/or there is a stray exeution pin that has not returned an ExitSequenceType!"), LastState ? *LastState->StateName.ToString() : TEXT("Unknown State"))
			; 
		} 
	}
	
	
	if (CurrentState){
		if (CurrentState->OnEnter(Owner) == EExitSequenceType::DEFAULT)
		{
			UE_LOG(
			LogTemp, 
			Error, 
			TEXT("%s's OnEnter function has not been overridden/or there is a stray exeution pin that has not returned an ExitSequenceType!"), CurrentState ? *CurrentState->StateName.ToString() : TEXT("Unknown State"))
			; 
		}
	}
}

void UFSMComponent::InitStates() {
	
	
	for (auto State : States)
	{
		if (State)
		{
			auto NewState = NewObject<UFSMState>(this, State);
			if (NewState)
			{
				NewState->InstanceStates();
				AllStates.Add(NewState);
			}
		}
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
	
	EExitSequenceType Result = CurrentState->OnRun(GetOwner());

	if (Result == EExitSequenceType::DEFAULT)
	{
		UE_LOG(
			LogTemp, 
			Error, 
			TEXT("%s's OnRun function has not been overridden/or there is a stray exeution pin that has not returned an ExitSequenceType!"), CurrentState ? *CurrentState->StateName.ToString() : TEXT("Unknown State"))
		; 
	}

	if (Result != EExitSequenceType::RUNNING)
	{
		UpdateCurrentState();
	}
	
}

void UFSMState::InstanceStates()
{
	
	
	for (auto Sub : Conditions)
	{
		if (Sub)
		{
			InstancedConditions.Add(NewObject<UCondition>(this, Sub));
		}
	}

	for (auto Consider : Considerations)
	{
		if (Consider)
		{
			InstancedConsiderations.Add(NewObject<UConsideration>(this, Consider));
		}
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
		if (State)
		{
			State->EvaluateConditions();
		}
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

