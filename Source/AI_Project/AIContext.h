// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/UserDefinedStruct.h"
#include "AIContext.generated.h"


/**
 * 
 */

USTRUCT(BlueprintType)
struct FAIContextStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasHunger;  
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsArmed; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerDistance; // if negative, can't see player
};

UCLASS(Blueprintable)
class AI_PROJECT_API UAIContext : public UActorComponent
{
	
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAIContextStruct ContextStruct;
};
/*
UCLASS()
class AI_PROJECT_API UAIContext : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};
*/