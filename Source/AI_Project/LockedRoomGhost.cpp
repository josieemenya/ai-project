// Fill out your copyright notice in the Description page of Project Settings.


 #include "LockedRoomGhost.h"

 #include "AIController.h"

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

	FGOAPGoal* OpenDoorGoal = new FGOAPGoal{
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
		AAIController* Controller = Cast<AAIController>(GetController());
		if (Controller)
			Controller->MoveToLocation(Door->GetActorLocation(), 10);
	};
}

// Called every frame
void ALockedRoomGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALockedRoomGhost::MoveToDoor()
{
	AAIController* Controller = Cast<AAIController>(GetController());
	if (Controller && Door)
	{
		Controller->MoveToLocation(Door->GetActorLocation(), 10.f);
	}
}

// Called to bind functionality to input
void ALockedRoomGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

