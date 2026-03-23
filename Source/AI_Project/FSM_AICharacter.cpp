// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_AICharacter.h"
#include "FMController.h"

// Sets default values
AFSM_AICharacter::AFSM_AICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFSM_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	FController = NewObject<AFMController>(this, FControllerClass); 
}

// Called every frame
void AFSM_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

// Called to bind functionality to input
void AFSM_AICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

