// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Misc/TVariant.h"
#include "WorldState.generated.h"


/**
 * 
 */


class ACharacter;
class AActor;


UENUM(BlueprintType)
enum class EGoapValueType : uint8
{
	INT = 0,
	FLOAT,
	BOOLEAN,
	ACTOR,
	OBJECT
};

USTRUCT(BlueprintType)
struct FGOAPValue
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGoapValueType Type;
	
	union GOAPValue
	{
		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = EditCondition = "Type == EGoapValueType::INT")
		int IntValue;
		
		//UPROPERTY()
	};
};

using GOAPValue = TVariant<int32, bool, float, AActor*, UObject*>; 

USTRUCT(BlueprintType)
struct FWorldState
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FGOAPValue> StateValues;
	
	FWorldState() = default;
	
	bool operator==(const FWorldState& Other) const;
	bool Satisfies(const FWorldState& Other);
	
	
	
};


