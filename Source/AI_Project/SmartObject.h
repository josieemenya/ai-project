// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Action.h"
#include "SmartObject.generated.h"



UCLASS()
class AI_PROJECT_API ASmartObject : public AActor
{
	GENERATED_BODY()
	
	static int InstanceNumber; 
public:	
	// Sets default values for this actor's properties
	ASmartObject();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartObjects)
	FName ObjectName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartObjects)
	TArray<UAction*> PossibleActions; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartObjects)
	int32 ObjectID; 
};
