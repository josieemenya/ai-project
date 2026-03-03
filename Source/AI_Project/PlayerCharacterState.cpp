// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterState.h"
#include "AbilitySystemComponent.h"
APlayerCharacterState::APlayerCharacterState()
{
    NetUpdateFrequency = 100.f; 
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ABS Component")); 
}

UAbilitySystemComponent* APlayerCharacterState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}
