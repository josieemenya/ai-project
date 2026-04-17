// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage.h"

#include "AI_ProjectCharacter.h"
#include "AI_ProjectGameMode.h"
#include "BaseAI.h"
#include "Blueprint/UserWidget.h"
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
	if (bMortis || DamageAmount <= 0) return;
	CharacterHealth = FMath::Max(0, CharacterHealth - DamageAmount);
	bMortis = (CharacterHealth == 0); 
	
	if (bMortis)
		OnDeath.Broadcast();
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
}

void UDamage::SetCharacterStamina(float stamina)
{
	if (bMortis || stamina <= 0 || stamina > CharacterMaxStamina) return;
	CharacterStamina = stamina;
}


void UDamage::HandleDeath()
{
	if (!GetOwner()->IsA(AAI_ProjectCharacter::StaticClass()))
	{
		// then handle non playable character things first and foremost
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Knockout");
	}
	if (!OnDeathScreen) return; 
	if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		UUserWidget* DScreen = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), OnDeathScreen);
		if (DScreen)
			DScreen->AddToViewport();
	} else if (auto Bot = Cast<ABaseAI>(GetOwner()))
	{
		// do bot death anim and others
	}
}
