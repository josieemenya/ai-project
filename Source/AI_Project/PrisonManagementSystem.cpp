// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "PrisonManagementSystem.h"
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
		URule* CreateRule = NewObject<URule>();
		AllRules.Add(CreateRule);
	}
}

bool URule::bIsRuleBroken_Implementation(FRuleContext Context)
{
	return true;
}

void URule::EstablishRuleBreak_Implementation(AAIController* ResultingController)
{
	
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
