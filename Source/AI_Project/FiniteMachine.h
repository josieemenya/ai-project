// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EFriendly : uint8
{
	None UMETA(DisplayName = "None", Hidden),
	Hostile =  1 << 0 UMETA(DisplayName = "Hostile"), // follow target, turn red?
	Friendly = 1 << 1 UMETA(DisplayName = "Friendly") // if friendly, then idk, turn green?
	
};

FORCEINLINE int32 operator&(int32 A, EFriendly B)
{
		return A & static_cast<int32>(B);
}

class AI_PROJECT_API FiniteMachine
{
public:
	FiniteMachine();
	~FiniteMachine();

	int32 State;
	EFriendly Friendly;

	void Update();
	virtual void OnHostile() = 0;
	virtual void OnFriendly() = 0;
};
