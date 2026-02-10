// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthAndStamina.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UHealthAndStamina : public UActorComponent
{
	GENERATED_BODY()

	float Health;
	float Stamina;

	float MaxHealth;
	float MaxStamina;

public:	
	// Sets default values for this component's properties
	UHealthAndStamina();
	int32 GetHealth() const { return Health; }
	int32 GetStamina() const { return Stamina; }

	void StaminaDrain(bool isRunning); // if shift is held down, then drain stamina, otherwise, regenerate stamina
	void StaminaRegen(bool isResting);

	void UpgradeHealth(float Amount);
	void UpgradeStamina(float Amount);

	void TakeDamage(float DamageAmount);
	void Heal(float HealAmount);

	void Die(){}; // ??
	void DecreaseStamina(float Amount);
	void RestoreStamina(float Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
