// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
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
	class UStaticMeshComponent* WeaponMesh; // weapon body
	
};

UCLASS()
class AI_PROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponInformation WeaponInformation; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
