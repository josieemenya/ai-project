// Fill out your copyright notice in the Description page of Project Settings.


#include "BTRobot.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

// Sets default values
ABTRobot::ABTRobot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USelector>("Root");
	Patrol = CreateDefaultSubobject<USequences>("Patrol"); 
	//Chase = CreateDefaultSubobject<USelector>("Chase");
	WalkTo = CreateDefaultSubobject<UMoveAction>("WalkTo");
	Condition = CreateDefaultSubobject<UConditionNode>("Condition");
}

// Called when the game starts or when spawned
void ABTRobot::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Controller: %s"), *GetNameSafe(GetController()));

	PC = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);  
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()); 
	FNavLocation NavLocation;
	Super::BeginPlay();
	Root->children.Add(Patrol);
	//Root->children.Add(Chase);
	Patrol->children.Add(WalkTo);
	WalkTo->Target = this;
	WalkTo->Condition = Condition;
	WalkTo->Condition->FCondition = GetDistanceTo(PC) < 4.0f; 
	if (NavSys && NavSys->GetRandomReachablePointInRadius(this->GetActorLocation(), 100.f, NavLocation))
		WalkTo->TargetLocation = NavLocation.Location;
	else 
		UE_LOG(LogTemp, Warning, TEXT("Couldn't move")); 
}

// Called every frame
void ABTRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Root->StatusRun();

}

// Called to bind functionality to input
void ABTRobot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

