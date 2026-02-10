// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HolyWater.generated.h"


USTRUCT(BlueprintType)
struct FHolyWaterData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Capacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TheHolyBottle; // the bottle that contains the holy water, can be used for throwing or something

	
	
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UHolyWater : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHolyWater();

	FHolyWaterData HolyWater;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
