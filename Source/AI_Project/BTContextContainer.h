// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BTContextContainer.generated.h"

/**
 * 
 */

UCLASS()
class AI_PROJECT_API UBTContextContainer : public UDataAsset
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bKnockedOut; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInCombat; 
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	
	
	
};
