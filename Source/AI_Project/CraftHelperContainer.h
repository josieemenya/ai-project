// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "CraftHelperContainer.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCraftHelperContainer 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCraftingItemData> CraftItemDetails;
};
