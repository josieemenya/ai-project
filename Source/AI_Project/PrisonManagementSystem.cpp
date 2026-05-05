// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "PrisonManagementSystem.h"

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
	
	for (TSubclassOf<URule> Rule : Settings->Rules)
	{
		if (!Rule)
		{
			continue;
		}
		URule* CreateRule = NewObject<URule>();
		AllRules.Add(CreateRule);
	}
	
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
}

void UPrisonManagementSystem::RemoveFlag(EPrisonState State)
{
	PrisonStateFlags &= ~static_cast<int32>(State);
}

void UPrisonManagementSystem::RemoveAllFlags()
{
	PrisonStateFlags = 0;
}

void UPrisonManagementSystem::OnRuleBreakOccured(const FRuleContext Context, AAIController* Controller)
{
	for (URule* Rule : AllRules)
	{
		if (Rule->bIsRuleBroken(Controller, Context))
		{
			int32 RandomIndex = FMath::RandRange(0, AllRules.Num() - 1);
			UGameplayStatics::PlaySoundAtLocation(Controller->GetWorld(), BreakSounds->RuleBreakSounds[RandomIndex], Controller->GetPawn()->GetActorLocation());  
			Rule->EstablishRuleBreak(Controller);
		}
	}
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
