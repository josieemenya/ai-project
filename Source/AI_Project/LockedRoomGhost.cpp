// Fill out your copyright notice in the Description page of Project Settings.


 #include "LockedRoomGhost.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Door.h"
#include "Key.h"
#include "FSMComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/EnumClassFlags.h"


//#include "Runtime/AIModule/Classes/AIController.h"

// Sets default values
ALockedRoomGhost::ALockedRoomGhost()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    AIControllerClass = AAIController::StaticClass();
	FiniteMachine = CreateDefaultSubobject<UFSMComponent>(TEXT("FSM"));


    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
}

// Called when the game starts or when spawned
void ALockedRoomGhost::BeginPlay()
{
	Super::BeginPlay();
	FiniteMachine->CurrentState = EAIState::IDLE;
	FiniteMachine->StateActions.Add(EAIState::IDLE, FSMAction{[]() { UE_LOG(LogTemp, Warning, TEXT("Idle...")); },"Idle"});
	FiniteMachine->StateActions.Add(EAIState::SEARCHING,  FSMAction{ [](){ UE_LOG(LogTemp, Warning, TEXT("Searching for key...")); }, "Searching" });
	FiniteMachine->StateActions.Add(EAIState::ATTACKING,  FSMAction{ [](){ UE_LOG(LogTemp, Warning, TEXT("Attacking!")); }, "Attacking" });
	FiniteMachine->StateActions.Add(EAIState::DEFENDING,  FSMAction{ [](){ UE_LOG(LogTemp, Warning, TEXT("Defending!")); }, "Defending" });
	FiniteMachine->AvailableStates = EAIState::IDLE | EAIState::SEARCHING | EAIState::ATTACKING | EAIState::DEFENDING;


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

	FSMUpdate();
}

void ALockedRoomGhost::FSMUpdate()
{
	
	if (!FiniteMachine) return;

	// This function can be used to update the FSM state based on conditions
	if (EnumHasAnyFlags(FiniteMachine->AvailableStates, FiniteMachine->CurrentState) && FiniteMachine->StateActions.Contains(FiniteMachine->CurrentState))
		FiniteMachine->StateActions[FiniteMachine->CurrentState].DoAction();
	else
		UE_LOG(LogTemp, Warning, TEXT("Current state is not available in FSM!"));

}

void ALockedRoomGhost::MoveToDoor()
{
	
}

// Called to bind functionality to input
void ALockedRoomGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

