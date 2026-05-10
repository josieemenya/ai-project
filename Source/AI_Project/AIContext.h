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
	float Opinion; // if hit or did quest update
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health; // heal dmg
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bKnockedOut; // if health zero true
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInCombat; // if dmg was instigated by a character
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LastKnownPlayerLocation; // if seen player
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistToPlayer; // seen player i guess
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsArmed; 
	
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