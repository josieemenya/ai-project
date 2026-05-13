// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "ExitSequence.generated.h"

/**
 * 
 */
UENUM( BlueprintType )
enum class EExitSequenceType : uint8
{
	SUCCESS,
	FAILURE,
	INTERRUPTED,
	INVALID,
	RUNNING,
	DEFAULT UMETA(Hidden)
};

