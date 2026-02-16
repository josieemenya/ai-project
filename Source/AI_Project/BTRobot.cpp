// Fill out your copyright notice in the Description page of Project Settings.


#include "BTRobot.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABTRobot::ABTRobot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USelector>("Root");
	Patrol = CreateDefaultSubobject<USequences>("Patrol"); 
	Chase = CreateDefaultSubobject<USelector>("Chase");
	WalkTo = CreateDefaultSubobject<UMoveAction>("WalkTo");
}

// Called when the game starts or when spawned
void ABTRobot::BeginPlay()
{
	Super::BeginPlay();
	Root->children.Add(Patrol);
	Root->children.Add(Chase);
	WalkTo->Target = this;
	PC = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);  
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

