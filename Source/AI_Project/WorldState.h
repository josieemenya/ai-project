// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WorldState.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EWorldValueType : uint8
{
	INT,
	BOOL, 
	FLOAT,
	VECTOR,
	ACTOR
};


USTRUCT(BlueprintType)
struct FWorldValue
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWorldValueType ValueType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition = "ValueType == EWorldValueType::INT"))
	int32 IntValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition = "ValueType == EWorldValueType::BOOL"))
	bool bValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition = "ValueType == EWorldValueType::FLOAT"))
	float FloatValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition = "ValueType == EWorldValueType::VECTOR"))
	FVector VectorValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition = "ValueType == EWorldValueType::ACTOR"))
	AActor* ActorValue;
	
};


USTRUCT(BlueprintType)
struct FWorldState
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWorldValue test; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, bool> StateValues;
	
	FWorldState() = default;
	
	bool operator==(const FWorldState& Other) const;
	
	bool Satisfies(const FWorldState& Other) const;
	
};
