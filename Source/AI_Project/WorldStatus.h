// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WorldStatus.generated.h"

/**
 * 
 */
// later burgers, bigger fish to fry
UENUM(BlueprintType)

enum EWorldStatusEvent
{
	NONE = 0, 
	UNARMED = 1 // yknow something like that, in the case that an AI is unaremd
};


UCLASS()
class AI_PROJECT_API AWorldStatus : public AGameStateBase
{
	GENERATED_BODY()
	
};
