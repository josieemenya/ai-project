// Fill out your copyright notice in the Description page of Project Settings.


 #include "LockedRoomGhost.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Door.h"
#include "Key.h"
#include "Kismet/GameplayStatics.h"

//#include "Runtime/AIModule/Classes/AIController.h"

// Sets default values
ALockedRoomGhost::ALockedRoomGhost()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALockedRoomGhost::BeginPlay()
{
	Super::BeginPlay();

	CurrentGoal = new FGOAPGoal{
		"OpenDoor",
		1,
		{
			{"DoorOpen", true}
		}
	};
	
	FGOAPAction* OpenLockedDoor = new FGOAPAction();
	OpenLockedDoor->Name = "OpenLockedDoor";
	OpenLockedDoor->Preconditions = {
		{"HasKey", true},
		{"DoorOpen", false}
	};
	OpenLockedDoor->Effects = {
		{"DoorOpen", true}
	};
	OpenLockedDoor->Perform = [this]()
	{
		Door = Cast<ADoor>(
			UGameplayStatics::GetActorOfClass(GetWorld(), ADoor::StaticClass())
		);
		AAIController* cController = Cast<AAIController>(GetController());
		if (cController && Door)
			cController->MoveToLocation(Door->GetActorLocation(), 10);
	};

	FGOAPAction* FoundKey = new FGOAPAction();
	FoundKey->Name = "FoundKey";
	FoundKey->Preconditions = {
		{"HasKey", false},
		{"SearchedForKey", false}
	};
	FoundKey->Effects = {
		{"HasKey", true},
	};
	FoundKey->Perform = [this]()
	{
		WorldState->StateValues["HasKey"] = true;
	};
	
	auto SearchForKey = new FGOAPAction();
	SearchForKey->Name = "SearchForKey";
	SearchForKey->Preconditions = {
		{"HasKey", false},
		{"SearchedForKey", false}
	};
	SearchForKey->Effects = {
		{"SearchedForKey", true},
	};
	SearchForKey->Perform = [this]()
	{
		DoorKey = Cast<AKiey>(
			UGameplayStatics::GetActorOfClass(GetWorld(), AKiey::StaticClass())
		);
		// key location
		if (!DoorKey)
			return;
		if (GetDistanceTo(DoorKey) < 100.f )
		{
			WorldState->StateValues["HasKey"] = true;
			return;
		}
		
		AAIController* cController = Cast<AAIController>(GetController());
		if (cController)
		{
			FNavLocation RandomPoint;
			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

			if (NavSys && NavSys->GetRandomReachablePointInRadius(
				GetActorLocation(),
				500.f,
				RandomPoint))
			{
				cController->MoveToLocation(RandomPoint.Location);
			}
		}
	};

	AvailableActions.AddUnique(std::move(OpenLockedDoor));
	AvailableActions.AddUnique(std::move(FoundKey));
	AvailableActions.AddUnique(std::move(SearchForKey));
}

// Called every frame
void ALockedRoomGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentAction && CurrentGoal)
	{
		TArray<FGOAPAction*> Plan = testGOAP::Plan(AvailableActions, WorldState, CurrentGoal);
		if (Plan.Num() > 0)
		{
			CurrentAction = Plan[0];
			CurrentAction->Perform();
		}
	}

}

void ALockedRoomGhost::MoveToDoor()
{
	
}

// Called to bind functionality to input
void ALockedRoomGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

