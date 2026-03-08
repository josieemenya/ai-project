// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAI.h"
//#include "LockAndKey.h"
#include "BlackboardSystem.h"
#include "PlannerComponent.h"
#include "Damage.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"


// Sets default values
ABaseAI::ABaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlannerComponent = CreateDefaultSubobject<UPlannerComponent>("PlannerComponent");
	DamageComp = CreateDefaultSubobject<UDamage>("Damage Component");
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
	if (PlannerComponent->ToDoStack.Num() > 0)
	{
		return;
	}
	
	if (!Goals.IsEmpty())
	{
		

		UE_LOG(LogTemp, Warning, TEXT("Planning for goal"));
		
		auto BaseCurrentState = CurrentState; 
		
		RegisterSeePlayer(PlannerComponent);
		PlannerComponent->UpdateSmartObjects(BaseCurrentState);
		UGoal* CurrentGoal = NewObject<UGoal>(this, Goals[0]);
		
    	if (CurrentGoal->bRequiresSmartObject && PlannerComponent->AllSmartObjectsNearby.Num() == 0)
    	{
        	UE_LOG(LogTemp, Warning, TEXT("Skipping planning: no smart objects yet"));
        	return;
   	 	}
		
		

		PlannerComponent->PlanGoal(BaseCurrentState, CurrentGoal->DesiredState);

		if (PlannerComponent->ToDoStack.Num() > 0)
		{
			Goals.RemoveAt(0);
		}
	}
}

// Called every frame
void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlannerComponent->ToDoStack.Num() > 0)
	{
		UpdateActions();
		return;
	}
	
	if (Goals.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Planning finished or Invalidates"));
		StartPlanning();
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
	if (PlannerComponent->ToDoStack.Num() > 0)
	{
		return;
	}

	PlannerComponent->UpdateSmartObjects(CurrentState);

	if (Goals.Num() > 0)
	{
		UGoal* CurrentGoal = NewObject<UGoal>(this, Goals[0]);

		StartPlanning(); 
	}
}

bool ABaseAI::RegisterSeePlayer(UPlannerComponent* Planner)
{
	TArray<AActor*> SeeActors;
	if (auto HasPerception = GetController()->FindComponentByClass<UAIPerceptionComponent>())
	{
		HasPerception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SeeActors);
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Perception Component"));
	}
	
	if (SeeActors.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Registering SeeActors"));
		auto Player = SeeActors.Find(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player != INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cam See Player"));
			CurrentState.StateValues.FindOrAdd("Player", true); 
			FBlackboardCustomEntry PlayerEntry = FBlackboardCustomEntry();
			PlayerEntry.EntryName = "Player";
			PlayerEntry.ValueType = EBlackboardKey::Actor;
			PlayerEntry.ActorValue = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			PlannerComponent->AIbBlackboardSystem->Blackboard->BlackboardEntries.Add(PlayerEntry);
			return true;
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("No player found"));
	}
	
	return false;
}
