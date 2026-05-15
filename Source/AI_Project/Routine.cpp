// Fill out your copyright notice in the Description page of Project Settings.


#include "Routine.h"
#include "ExitSequence.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "GPController.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"


// Sets default values for this component's properties
URoutineComponent::URoutineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}


void URoutineComponent::BeginPlay()
{
	Super::BeginPlay();
	TimeSystem = Cast<UTimeSystem>(GetWorld()->GetSubsystem<UTimeSystem>());
	FString ContextString; 
	
	if (RoutineTable){
		RoutineTable->GetAllRows<FRoutineElem>(ContextString, AllRoutines); 
	} else {
		// log it
		
		UE_LOG(LogTemp, Error, TEXT("No routine table found"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Routine Size : %d"), AllRoutines.Num())
}


FRoutineState URoutineComponent::GetCurrentState()
{
	FTimeData Data = TimeSystem->GetTimeData();
	//FRoutineState Result;

	for (auto State : AllRoutines)
	{
		if (WithinTimeRange(Data, State->Range))
		{
			return State->State;
		}
	}

	return FRoutineState();
}

EExitSequenceType URoutineComponent::TransitionRoutine()
{
	LastCurrentState = CurrentState;
	CurrentState = GetCurrentState();

	AAIController* Steer = Cast<AAIController>(GetOwner()); // get controller; 

	
	float AcceptableRadius = 50.f; 
	if (!Steer)
	{
		UE_LOG(LogTemp, Error, TEXT("No controller accessible"));
		return EExitSequenceType::FAILURE;
	}

	ACharacter* AttachedActor = Cast<ACharacter>(Steer->GetPawn());

	if (!AttachedActor)
	{
		UE_LOG(LogTemp, Error, TEXT("No attached character accessible"));
		return EExitSequenceType::FAILURE;
	}
	
	if (Steer)
	{
		auto SteerResult = Steer->GetMoveStatus(); 
		
		float Dist = FMath::Abs((AttachedActor->GetActorLocation() - CurrentState.Location).Length()); 
		
		if (Dist <= AcceptableRadius || FMath::IsNearlyEqual(Dist, AcceptableRadius, 2))
		{
			return EExitSequenceType::SUCCESS; 
		} 
		
		switch (SteerResult)
		{
		case EPathFollowingStatus::Idle:
			Steer->MoveToLocation(CurrentState.Location, AcceptableRadius); 
			return EExitSequenceType::RUNNING;

		case EPathFollowingStatus::Moving:
			return EExitSequenceType::RUNNING;

		case EPathFollowingStatus::Waiting:
		case EPathFollowingStatus::Paused:
			return EExitSequenceType::RUNNING; 
		}
		
	}

	UE_LOG(LogTemp, Error, TEXT("Default Dialogue reached."))
	return EExitSequenceType::FAILURE;
}

EExitSequenceType URoutineComponent::RoutineSequence()
{
	// if at location 

	// get mesh 
	AGPController* AttachedController = Cast<AGPController>(GetOwner());

	if (!AttachedController)
	{
		UE_LOG(LogTemp, Error, TEXT("No attached character accessible"));
		return EExitSequenceType::FAILURE;
	}

	USkeletalMeshComponent* Skelly = Cast<ACharacter>(AttachedController->GetPawn())->GetMesh();

	if (!Skelly)
	{
		UE_LOG(LogTemp, Error, TEXT("No Skeleton character accessible"));
		return EExitSequenceType::FAILURE;
	}

	UAnimInstance* Montage = Skelly->GetAnimInstance();

	if (!Montage)
	{
		UE_LOG(LogTemp, Error, TEXT("No Montage object accessible"));
		return EExitSequenceType::SUCCESS; //return Success for now
	}

	if (!Montage->Montage_IsPlaying(CurrentState.Animation))
	{
		Montage->Montage_Play(CurrentState.Animation);
	}


	if (!bHasStateChanged()) // actually it runs until the current schedulr is over
	{
		return EExitSequenceType::RUNNING;
	}

	CurrentState.Animation->bLoop = false;

	return EExitSequenceType::SUCCESS;
}

bool URoutineComponent::bHasStateChanged() const
{
	return LastCurrentState != CurrentState;
}

bool URoutineComponent::WithinTimeRange(const FTimeData& Data, const FTimeRange& TimeRange) const
{
	int Current = Data.Hour * 60 + Data.Minute;
	int Start   = TimeRange.StartHourRange * 60 + TimeRange.StartMinuteRange;
	int End     = TimeRange.EndHourRange   * 60 + TimeRange.EndMinuteRange;

	return Current >= Start && Current <= End;
}

void URoutineComponent::MoveSuccessful(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result != EPathFollowingResult::Success)
	{
		TransitionRoutine(); 
	}
}
