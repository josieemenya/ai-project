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
	CurrentState = FWorldState();
	CurrentState.StateValues = TMap<FString, bool>();
	
	PlannerComponent->OnPlanInvalid.AddUObject(this, &ABaseAI::Replan);
	GetWorldTimerManager().SetTimerForNextTick(this, &ABaseAI::StartPlanning);
	
	
}

void ABaseAI::StartPlanning()
{
	
	if (!Goals.IsEmpty())
	{
		PlannerComponent->PlanGoal(CurrentState, Goals[0]->DesiredState);
		if (PlannerComponent->ToDoStack.Num() > 0)
			Goals.RemoveAt(0);
	}
}

// Called every frame
void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlannerComponent->ToDoStack.Num() > 0)
		UpdateActions();
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
	PlannerComponent->UpdateSmartObjects(CurrentState);

	if (Goals.Num() > 0)
	{
		PlannerComponent->PlanGoal(CurrentState, Goals[0]->DesiredState);
	}
}

