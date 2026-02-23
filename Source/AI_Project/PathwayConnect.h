// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathwayConnect.generated.h"

UENUM(BlueprintType)
enum class EDir : uint8
{
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3,
}; // is this actually needed?? 


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UPathwayConnect : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathwayConnect();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	EDir ConnectingDirection; // idk if we need a pathway to that
	ULevel* Level;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
};
