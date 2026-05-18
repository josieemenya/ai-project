// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WorldState.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FWorldState
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WorldSTate")
	TMap<FString, bool> StateValues;
	
	FWorldState() = default;
	
	bool operator==(const FWorldState& Other) const;
	
	bool SatisfiesAll(const FWorldState& Other) const;
	bool SatisfiesAny(const FWorldState& WorldState) const;
};
