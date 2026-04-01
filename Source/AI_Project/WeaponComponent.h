// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UENUM(BlueprintType)
enum class EWeaponType
{
	GUN, // mainly for line trace 
	DAGGER
};


USTRUCT(BlueprintType)
struct FWeaponInformation
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponName; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* WeaponMesh; // weapon body
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWeaponInformation> Weapons;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponInformation CurrentlyEquippedWeapon; 
	
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
