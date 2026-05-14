// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/ActorComponent.h"
#include "Damage.generated.h"


class AAIController;
class UUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UDamage : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamage();
	
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
	float CharacterHealth; 
	
	UPROPERTY( EditDefaultsOnly )
	float CharacterMaxHealth;
	
	UPROPERTY( EditDefaultsOnly )
	float CharacterStamina; 
	
	UPROPERTY( EditDefaultsOnly )
	float CharacterMaxStamina;
	
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* Montage;
	
	UPROPERTY(BlueprintReadOnly)
	bool bMortis; 
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AController*, ParentController);

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OnDeathScreen; 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	void UnlockMovement(); 
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION( BlueprintPure )
	float GetCharacterHealth() const; 
	
	UFUNCTION( BlueprintPure )
	float GetCharacterMaxHealth() const;
	
	UFUNCTION( BlueprintPure )
	float GetCharacterStamina() const;
	
	UFUNCTION( BlueprintPure )
	float GetCharacterMaxStamina() const;
	
	UFUNCTION( BlueprintCallable )
	void HealHealth( float health );
	
	UFUNCTION( BlueprintCallable )
	void HealStamina( float stamina );
	
	UFUNCTION( BlueprintCallable )
	void UpdateMaxHealth( float maxHealth );
	
	UFUNCTION( BlueprintCallable )
	void UpdateMaxStamina( float stamina );
	
	UFUNCTION( BlueprintCallable )
	void DamageHealth( float health );
	
	UFUNCTION( BlueprintCallable )
	void DamageStamina( float stamina );
	
	UFUNCTION( BlueprintCallable )
	void SetCharacterHealth( float health );
	
	UFUNCTION( BlueprintCallable )
	void SetCharacterStamina( float stamina );
	
	UFUNCTION( BlueprintCallable )
	void HandleDeath(AController* CharacterController); 
	
	
	UPROPERTY( BlueprintAssignable )
	FOnDeath OnCharacterDeath; 
};

inline float UDamage::GetCharacterHealth() const
{
	return CharacterHealth;
}

inline float UDamage::GetCharacterStamina() const
{
	return CharacterStamina;
}

inline float UDamage::GetCharacterMaxStamina() const
{
	return CharacterMaxStamina;
}

inline float UDamage::GetCharacterMaxHealth() const
{
	return CharacterMaxHealth; 
}