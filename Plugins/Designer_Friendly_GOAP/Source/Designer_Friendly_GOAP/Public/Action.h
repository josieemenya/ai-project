// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WorldState.h"
#include "ExitSequence.h"
#include "Action.generated.h"

/**
 * 
 */

class ASmartObject; 


class UWorldState;

UCLASS(Blueprintable, BlueprintType)
class DESIGNER_FRIENDLY_GOAP_API UAction : public UDataAsset
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* OwningActor; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name; // the name of the action, used for debugging and identification
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWorldState Preconditions;

	UFUNCTION(BlueprintNativeEvent)
	EExitSequenceType Execute(AActor* Owner);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWorldState Effects; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost; // the cost of performing the action, used for planning
	
	UAction() = default;
	
	bool operator==(const UAction& Other) const
	{
		return Name == Other.Name; // or whatever defines equality
	}
	
	
};
