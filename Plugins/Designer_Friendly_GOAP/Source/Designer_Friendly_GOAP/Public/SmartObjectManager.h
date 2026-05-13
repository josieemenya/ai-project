// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SmartObject.h"
#include "SmartObjectManager.generated.h"

/**
 * 
 */
UCLASS()
class DESIGNER_FRIENDLY_GOAP_API USmartObjectManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	public:
	//UPROPERTY(EditAnywhere)
	TMap<int32, ASmartObject*> SmartObjects;
};
