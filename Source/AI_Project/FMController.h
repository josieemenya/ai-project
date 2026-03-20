// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FSMComponent.h"
#include "FMController.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API AFMController : public AAIController
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UFSMComponent> FSMComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBlackboardComponent* BBlackboard;
	
};


