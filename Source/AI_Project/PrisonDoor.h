// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.h"
#include "PrisonDoor.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;

UCLASS()
class AI_PROJECT_API APrisonDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrisonDoor();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCapsuleComponent* Capsule;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsOpen;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class AAI_ProjectCharacter* InteractingPlayerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DoorKeyName; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundWave* UnlockDoorSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundWave* LockDoorSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapCapsule(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnClickDoor(AActor* TouchedActor, FKey Key);
	
	UFUNCTION()
	void LockDoor(); 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
		FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
