// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedAmmo.generated.h"


UCLASS()
class URangedAmmoPool : public UGameInstanceSubsystem
{
	GENERATED_BODY()


	TArray<AActor*> Ammo;
	const int MaxPoolSize = 30;

public:
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};

UCLASS()
class AI_PROJECT_API ARangedAmmo : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARangedAmmo();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* AmmoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InjuredCombatant(UPrimitiveComponent* HitComponent,
	                      AActor* OtherActor,
	                      UPrimitiveComponent* OtherComp,
	                      FVector NormalImpulse,
	                      const FHitResult& Hit);
};
