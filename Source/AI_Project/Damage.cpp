// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage.h"

#include "AIController.h"
#include "AI_ProjectCharacter.h"
#include "AI_ProjectGameMode.h"
#include "BaseAI.h"
#include "GPController.h"
#include "PrisonManagementSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProfilingDebugging/CookStats.h"

// Sets default values for this component's properties
UDamage::UDamage()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CharacterHealth = 100;
	CharacterMaxHealth = 100;
	CharacterMaxStamina = 100;
	CharacterStamina = 100;
	bMortis = false;
}


// Called when the game starts
void UDamage::BeginPlay()
{
	Super::BeginPlay();

	OnDeath.AddDynamic(this, &UDamage::HandleDeath);
	AActor* AttachedActor = Cast<AActor>(GetOwner());
	if (auto GetCharacterRef = Cast<ACharacter>(AttachedActor))
	{
		if (AGPController* CharacterController = Cast<AGPController>(GetCharacterRef->GetController()))
		{
			CharacterController->Planner->BB_Planner->SetValueAsFloat("Health", CharacterMaxHealth);
			CharacterController->Planner->BB_Planner->SetValueAsFloat("Opinion", 50);
		}
	}
}

void UDamage::UnlockMovement()
{
	if (AAIController* AIController = Cast<AAIController>(GetOwner()))
	{
		if (ACharacter* Character = Cast<ACharacter>(AIController->GetPawn()))
		{
			Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			CharacterHealth = CharacterMaxHealth;
			AIController->GetBlackboardComponent()->SetValueAsFloat("Health", CharacterHealth);
			AIController->GetBlackboardComponent()->SetValueAsBool("KnockedOut", false); 
			AIController->GetBlackboardComponent()->SetValueAsBool("InCombat", false);
			AIController->GetBlackboardComponent()->SetValueAsBool("Engaged", false);
		}
	}

	// Delay this until AFTER animation recovery if needed
	bMortis = false; 
}


// Called every frame
void UDamage::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamage::HealHealth(float health)
{
	if (bMortis || health <= 0) return;
	CharacterHealth = FMath::Min(CharacterHealth + health, CharacterMaxHealth);
	
	if (AAIController* CharacterController = Cast<AAIController>(GetOwner()))
	{
		CharacterController->GetBlackboardComponent()->SetValueAsFloat("Health", CharacterHealth);
	}
}

void UDamage::HealStamina(float stamina)
{
	if (bMortis || stamina <= 0) return;
	CharacterStamina = FMath::Min(CharacterStamina + stamina, CharacterMaxStamina);
}

void UDamage::UpdateMaxHealth(float maxHealth)
{
	if (bMortis || maxHealth <= 0) return;
	CharacterMaxHealth += maxHealth;
}

void UDamage::UpdateMaxStamina(float maxStamina)
{
	if (bMortis || maxStamina <= 0) return;
	CharacterMaxStamina += maxStamina;
}

void UDamage::DamageHealth(float DamageAmount)
{
	
	AActor* AttachedActor = GetOwner();
	
	if (bMortis || DamageAmount <= 0) return; // if character is already dead and or damage is negligent
	CharacterHealth = FMath::Max(0, CharacterHealth - DamageAmount);
	bMortis = (CharacterHealth <= 0.f); 
	
	if (auto GetCharacterRef = Cast<ACharacter>(AttachedActor))
	{
		if (AGPController* CharacterController = Cast<AGPController>(GetCharacterRef->GetController()))
		{	
			CharacterController->Planner->BB_Planner->SetValueAsFloat("Health", CharacterHealth);
			CharacterController->Planner->BB_Planner->SetValueAsBool("InCombat", true);
			if (bMortis)
			{
				CharacterController->Planner->BB_Planner->SetValueAsBool("KnockedOut", bMortis);
				OnCharacterDeath.Broadcast(CharacterController);
			}
		}
	}
	
	if (bMortis)
	{
		if (AAIController* CharacterController =  Cast<AAIController>(GetOwner()))
		{
			OnDeath.Broadcast(CharacterController);
		} else {
			ACharacter* PCharacter = Cast<ACharacter>(GetOwner());
			OnDeath.Broadcast(Cast<AController>(PCharacter->GetController()));
		}
	}
}

void UDamage::DamageStamina(float stamina)
{
	if (bMortis || stamina <= 0) return;
	CharacterStamina = FMath::Max(0, CharacterStamina - stamina);
	//bMortis = (CharacterStamina == 0);
}

void UDamage::SetCharacterHealth(float health)
{
	if (bMortis || health <= 0 || health > CharacterMaxHealth) return;
	CharacterHealth = health;
	
	if (AAIController* CharacterController = Cast<AAIController>(GetOwner()))
	{
		CharacterController->GetBlackboardComponent()->SetValueAsFloat("Health", CharacterHealth);
	}
}

void UDamage::SetCharacterStamina(float stamina)
{
	if (bMortis || stamina <= 0 || stamina > CharacterMaxStamina) return;
	CharacterStamina = stamina;
}


void UDamage::HandleDeath(AController* CharacterController)
{
	GEngine->AddOnScreenDebugMessage(33, 2.f, FColor::Yellow, "BRPPPPP");
	if (AAIController* AICharacter = Cast<AAIController>(CharacterController))
	{
		GEngine->AddOnScreenDebugMessage(33, 2.f, FColor::Yellow, "DoUnlockMovement");
		FTimerHandle TimerHandle;
		
		AICharacter->GetBlackboardComponent()->SetValueAsBool("KnockedOut", true);
		AICharacter->GetBlackboardComponent()->SetValueAsBool("Engaged", false);
		
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UDamage::UnlockMovement,
			10.f,
			false
		);
	} else
	{
		if ((GetWorld()->GetSubsystem<UPrisonManagementSystem>()->PrisonStateFlags & static_cast<int32>(EPrisonState::LOCKDOWN)) != 0)
		{
			APlayerController* PC = GetWorld()->GetFirstPlayerController();

			if (PC && OnDeathScreen)
			{
				UUserWidget* DScreen = CreateWidget<UUserWidget>(PC, OnDeathScreen);

				if (!DScreen)
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to create death widget"));
					return;
				}

				DScreen->AddToViewport();
				
			}
		} // else get cutscene, refresh everything, clear chest and everything with contraband
		else
		{
			
		}
	} 
}
