// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pathways.generated.h"

class UBoxComponent; 
class UStaticMeshComponent;

UCLASS()
class AI_PROJECT_API APathways : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathways();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* VisiblePath; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ConnectedRoom; 
};
