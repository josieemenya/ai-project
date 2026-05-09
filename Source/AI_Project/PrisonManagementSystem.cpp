// Fill out your copyright notice in the Description page of Project Settings.
#include "PrisonManagementSystem.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "PrisonRules.h"
#include "Components/Slider.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/AudioComponent.h"


void UPrisonUI::UpdateLevelMusicVolume(float Val)
{
	float VolumeLevel = Val / 100; 
	if (AudioComponent)
	{
		AudioComponent->SetVolumeMultiplier(VolumeLevel);
	}
}

void UPrisonUI::NativeConstruct()
{
	Super::NativeConstruct();
	AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), Cast<USoundBase>(LevelMusic)); 
}

void UOptionItemSlider::UpdateVolume(float Val)
{
	switch (OptionType)
	{
		case EOptionValueType::VOLUME:
			PrisonRef->UpdateLevelMusicVolume(Val);
		break;
		
		default:
			break;
	}
	 
}

void UOptionItemSlider::NativeConstruct()
{
	Super::NativeConstruct();
	TArray<UUserWidget*> SpecificWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), SpecificWidgets,UPrisonUI::StaticClass()); 
	
	PrisonRef = Cast<UPrisonUI>(SpecificWidgets[0]);
	SliderVariable->OnValueChanged.AddDynamic(this, &UOptionItemSlider::UpdateVolume); 
}

void UPrisonManagementSystem::InitializeRulesFromSettings()
{
	const UDataContainerSettings* Settings = GetDefault<UDataContainerSettings>(); 
	
	
	if (Settings->BrokenRules){
		BreakSounds = NewObject<URuleBreakSound>(this, Settings->BrokenRules);
	}
}
void UPrisonManagementSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeRulesFromSettings(); 
}

void UPrisonManagementSystem::AddFlag(EPrisonState State)
{
	PrisonStateFlags |= static_cast<int32>(State);
	UE_LOG(LogTemp, Warning, TEXT("PrisonFlagsValue: %d"), PrisonStateFlags)
}

void UPrisonManagementSystem::RemoveFlag(EPrisonState State)
{
	PrisonStateFlags &= ~static_cast<int32>(State);
}

void UPrisonManagementSystem::RemoveAllFlags()
{
	PrisonStateFlags = 0;
}

void UPrisonManagementSystem::PrintPrisonState()
{
	UE_LOG(LogTemp, Warning, TEXT("-----"))
	UE_LOG(LogTemp, Warning, TEXT("Lockdown : %d"), (PrisonStateFlags & (int32)EPrisonState::LOCKDOWN ))
	UE_LOG(LogTemp, Warning, TEXT("Routine : %d"), (PrisonStateFlags & (int32)EPrisonState::ROUTINE ))
	UE_LOG(LogTemp, Warning, TEXT("Takeover : %d"), (PrisonStateFlags & (int32)EPrisonState::TAKEOVER ))
	UE_LOG(LogTemp, Warning, TEXT("Lightsout : %d"), (PrisonStateFlags & (int32)EPrisonState::LIGHTSOUT ))
	UE_LOG(LogTemp, Warning, TEXT("------"))
}


/*void UPrisonManagementSystem::OnRuleBreakOccured()
{
	for (URule* Rule : AllRules)
	{
		if (Rule->bIsRuleBroken())
		{
			//
		}
	}
}*/
