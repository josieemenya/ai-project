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
protected:
	void BeginPlay() override;

private:
	GENERATED_BODY()

public:
	
	AFMController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBlackboardComponent* BBlackboard;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFSMComponent* Comp;
};


