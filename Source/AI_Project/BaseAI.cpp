// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAI.h"
//#include "LockAndKey.h"
#include "PlannerComponent.h"
#include "Perception/AIPerceptionComponent.h"


// Sets default values
ABaseAI::ABaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlannerComponent = CreateDefaultSubobject<UPlannerComponent>("PlannerComponent");
}

// Called when the game starts or when spawned
void ABaseAI::BeginPlay()
{
	Super::BeginPlay();
	//Lock = new LockAndKey();
	
	PlannerComponent->OnPlanInvalid.AddUObject(this, &ABaseAI::Replan);
	GetWorldTimerManager().SetTimerForNextTick([this](){
		PlannerComponent->UpdateSmartObjects(CurrentState);
		StartPlanning();
	});
	
}

void ABaseAI::StartPlanning()
{
	if (bIsPlanning) return;
    bIsPlanning = true;

	if (!Goals.IsEmpty() & IsValid(Goals[0].Get()))
	{

		UGoal* CurrentGoal = NewObject<UGoal>(this, Goals[0]);

		UE_LOG(LogTemp, Warning, TEXT("Planning for goal"));
	
		PlannerComponent->UpdateSmartObjects(CurrentState);
		PlannerComponent->PlanGoal(CurrentState, CurrentGoal->DesiredState);

		if (PlannerComponent->ToDoStack.Num() > 0)
		{
			Goals.RemoveAt(0);
		}
	}
	bIsPlanning = false;
}

// Called every frame
void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlannerComponent->ToDoStack.Num() > 0)
		UpdateActions();
	
	if (PlannerComponent->ToDoStack.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Planning failed"));
	}
}

void ABaseAI::UpdateActions()
{
	PlannerComponent->UpdateStack(this); 
}

// Called to bind functionality to input
void ABaseAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseAI::Replan()
{
	if (bIsPlanning) return;
	
	bIsPlanning = true;

	PlannerComponent->UpdateSmartObjects(CurrentState);

	if (Goals.Num() > 0)
	{
		UGoal* CurrentGoal = NewObject<UGoal>(this, Goals[0]);

		PlannerComponent->PlanGoal(CurrentState, CurrentGoal->DesiredState);
	}
	
	bIsPlanning = false;

}

