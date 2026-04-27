// Fill out your copyright notice in the Description page of Project Settings.


#include "Routine.h"
#include "ExitSequence.h"
#include "GameFramework/Character.h"
#include "AIController.h"
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
	TimeSystem = Cast<UTimeSystem>(GetWorld()->GetGameInstance());
}


FRoutineState URoutineComponent::GetCurrentState()
{
	FTimeData Data = TimeSystem->GetTimeData();
	FRoutineState Result;

	for (auto State : AllRoutines)
	{
		if (WithinHourRange(Data, State.Range) && WithinMinuteRange(Data, State.Range))
		{
			Result = State.State;
		}
	}

	return Result;
}

EExitSequenceType URoutineComponent::TransitionRoutine()
{
	LastCurrentState = CurrentState;
	CurrentState = GetCurrentState();

	AAIController* Steer = Cast<AAIController>(GetOwner()); // get controller; 
	
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
	
	
	/*FNavLocation Projected;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSys || !NavSys->ProjectPointToNavigation(CurrentState.Location, Projected))
	{
		UE_LOG(LogTemp, Error, TEXT("State location invalid even after projection"));
		return EExitSequenceType::FAILURE;
	}*/
	
	CurrentState.Location = FVector{ GetOwner()->GetActorLocation().X + 100.f, GetOwner()->GetActorLocation().Y, GetOwner()->GetActorLocation().Z};

	

	if (Steer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target: %s"), *CurrentState.Location.ToString());
		switch (Steer->MoveToLocation(CurrentState.Location, 10.f))
		{
		case EPathFollowingRequestResult::AlreadyAtGoal:
			return EExitSequenceType::SUCCESS;

		case EPathFollowingRequestResult::RequestSuccessful:
			return EExitSequenceType::RUNNING;

		case EPathFollowingRequestResult::Failed:
			UE_LOG(LogTemp, Error, TEXT("Path Following Request Failed"));
			return EExitSequenceType::FAILURE;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Default Dialogue reached."))
	return EExitSequenceType::FAILURE;
}

EExitSequenceType URoutineComponent::RoutineSequence()
{
	// if at location 

	// get mesh 
	ACharacter* AttachedActor = Cast<ACharacter>(GetOwner());

	USkeletalMeshComponent* Skelly = AttachedActor->GetMesh();

	if (!Skelly)
	{
		return EExitSequenceType::FAILURE;
	}

	UAnimInstance* Montage = Skelly->GetAnimInstance();

	if (!Montage)
	{
		return EExitSequenceType::FAILURE;
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

bool URoutineComponent::WithinHourRange(const FTimeData& Data, const FTimeRange& TimeRange) const
{
	return (Data.Hour >= TimeRange.StartHourRange && Data.Hour <= TimeRange.EndHourRange);
}

bool URoutineComponent::WithinMinuteRange(const FTimeData& Data, const FTimeRange& TimeRange) const
{
	return (Data.Minute >= TimeRange.StartMinuteRange && Data.Minute <= TimeRange.EndMinuteRange);
}
