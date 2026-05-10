// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCharacter.h"
#include "BTController.h"

// Sets default values
ABTCharacter::ABTCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABTCharacter::BeginPlay()
{
	Super::BeginPlay();
	BTController = NewObject<ABTController>(this, ControllerClass);
}

// Called every frame
void ABTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

