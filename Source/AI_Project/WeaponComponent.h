// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


class AWeapon; 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AWeapon>> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AWeapon*> InstancedWeapons;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWeapon* CurrentlyEquippedWeapon; 
	
	void InitializeWeapons();
	
	UFUNCTION(BlueprintCallable)
	void Equip();  // maybe?
	
	UFUNCTION(BlueprintCallable)
	void Aim(); // if gun 
	
	UFUNCTION(BlueprintCallable)
	void Attack(); 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
