// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerCharacterState.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AI_PROJECT_API APlayerCharacterState : public APlayerState
{
	GENERATED_BODY()
public:
	APlayerCharacterState();
	
public:
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const; 
};
