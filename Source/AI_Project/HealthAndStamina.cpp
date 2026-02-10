// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAndStamina.h"



// Sets default values for this component's properties
UHealthAndStamina::UHealthAndStamina()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHealthAndStamina::StaminaDrain(bool isRunning)
{
	if (isRunning)
	{
		DecreaseStamina(1.0f); // decrease stamina by 1 per second
	}
	else
	{
		StaminaRegen(true); // regenerate stamina when not running
	}
}

void UHealthAndStamina::StaminaRegen(bool isRunning)
{
	if (isRunning)
	{
		Stamina += 1.0f; // regenerate stamina by 1 per second
		Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina); 
	}
}

void UHealthAndStamina::UpgradeHealth(float Amount)
{
	MaxHealth += Amount;
	Health = MaxHealth; // restore health to max when upgrading
}

void UHealthAndStamina::UpgradeStamina(float Amount)
{
	MaxStamina += Amount;
	Stamina = MaxStamina; // restore stamina to max when upgrading
}

void UHealthAndStamina::TakeDamage(float DamageAmount)
{
	Health -= DamageAmount; // that's it, that's the function, just decrease health by the damage amount
	if (Health <= 0)
	{
		Die(); // if health is 0 or less, then die
	}
}

void UHealthAndStamina::Heal(float HealAmount)
{
	Health += HealAmount;
}

void UHealthAndStamina::DecreaseStamina(float Amount)
{
	Stamina -= Amount;
}

void UHealthAndStamina::RestoreStamina(float Amount)
{
	Stamina += Amount;
	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina); // clamp stamina to max
}


// Called when the game starts
void UHealthAndStamina::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthAndStamina::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

