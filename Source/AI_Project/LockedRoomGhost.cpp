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
    AIControllerClass = AAIController::StaticClass();

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ALockedRoomGhost::BeginPlay()
{
	Super::BeginPlay();

	WorldState = new FWorldState();
	WorldState->StateValues.Add("HasKey", false);
	WorldState->StateValues.Add("SearchedForKey", false);
	WorldState->StateValues.Add("DoorOpen", false);


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
		
	auto SearchForKey = new FGOAPAction();
	SearchForKey->Preconditions = { {"HasKey", false}, {"SearchedForKey", false} };
	SearchForKey->Effects = { {"SearchedForKey", true} };
	SearchForKey->Perform = [this]() { WorldState->StateValues["SearchedForKey"] = true; };

	FoundKey->Preconditions = { {"HasKey", false}, {"SearchedForKey", true} };
	FoundKey->Effects = { {"HasKey", true}, {"SearchedForKey", true} };
	FoundKey->Perform = [this]() { WorldState->StateValues["HasKey"] = true; WorldState->StateValues["SearchedForKey"] = true; };

		AvailableActions.AddUnique(std::move(OpenLockedDoor));
		AvailableActions.AddUnique(std::move(FoundKey));
		AvailableActions.AddUnique(std::move(SearchForKey));
	}

// Called every frame
void ALockedRoomGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentGoal)
		return;

	// If no current action, plan a new sequence
	if (!CurrentAction)
	{
		TArray<FGOAPAction*> Plan = testGOAP::Plan(AvailableActions, WorldState, CurrentGoal);
		if (Plan.Num() > 0)
		{
			CurrentAction = Plan[0];

			// If it's a movement action, just start moving
			CurrentAction->Perform();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No plan found!"));
		}
		return;
	}

	// Track progress for movement actions
	AAIController* cController = Cast<AAIController>(GetController());
	if (CurrentAction->Name == "OpenLockedDoor" && cController && Door)
	{
		if (GetDistanceTo(Door) < 100.f) // target reached
		{
			UE_LOG(LogTemp, Warning, TEXT("Door reached! Action done."));
			WorldState->StateValues["DoorOpen"] = true; // mark effect
			CurrentAction = nullptr; // ready for next action
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

