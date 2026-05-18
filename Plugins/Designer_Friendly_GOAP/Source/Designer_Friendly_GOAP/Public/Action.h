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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action")
	AActor* OwningActor; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action")
	FName Name; // the name of the action, used for debugging and identification
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action")
	FWorldState Preconditions;

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	EExitSequenceType Execute(AActor* Owner);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action")
	FWorldState Effects; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action")
	float Cost; // the cost of performing the action, used for planning
	
	UAction() = default;
	
	bool operator==(const UAction& Other) const
	{
		return Name == Other.Name; // or whatever defines equality
	}
	
	
};
