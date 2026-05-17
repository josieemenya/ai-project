// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleController.h"

#include "PlannerComponent.h"
#include "UtilityWorldSystem.h"


// Sets default values
AExampleController::AExampleController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Planner = CreateDefaultSubobject<UPlannerComponent>("Planner");
	Reasoner = CreateDefaultSubobject<UUtilityReasoner>("Reasoner");
}

// Called when the game starts or when spawned
void AExampleController::BeginPlay()
{
	Super::BeginPlay();
	Reasoner->Initialize(this, Planner); 
}

// Called every frame
void AExampleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

