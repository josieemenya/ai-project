// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BTController.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API ABTController : public AAIController
{
	GENERATED_BODY()
public:
	
	ABTController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBlackboardComponent* BBlackboard;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTreeComponent* Comp;
	
};
