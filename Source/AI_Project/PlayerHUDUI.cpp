// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDUI.h"

#include "Damage.h"
#include "TimeSystem.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"

void UStatBarUI::NativeConstruct()
{
	Super::NativeConstruct();
	if (OwningPawn)
	{
		DamageComp = OwningPawn->GetController()->FindComponentByClass<UDamage>();
	}

	if (PlayerHealthBar)
	{
		PlayerHealthBar->PercentDelegate.BindUFunction(
			this,
			FName("GetHealthPercent"));
		PlayerHealthBar->SynchronizeProperties();
	}
}

float UStatBarUI::GetHealthPercent() const
{
	if (!DamageComp || DamageComp->CharacterMaxHealth <= 0.f)
	{
		return 0.f;
	}

	GEngine->AddOnScreenDebugMessage(233, 12, FColor::Yellow, FString::Printf(TEXT("Health: %f"), DamageComp->CharacterHealth)); 
	
	return DamageComp->CharacterHealth /
		   DamageComp->CharacterMaxHealth;
}

void UPlayerHUDUI::NativeConstruct()
{
	Super::NativeConstruct();
	FSlateFontInfo FontInfo;
	
	FontInfo.FontObject = TimeFont; 
	HourTextBlock->SetFont(FontInfo);
	TenthMinuteTextBlock->SetFont(FontInfo);
	MinuteTextBlock->SetFont(FontInfo);

	UTimeSystem* Time = Cast<UTimeSystem>(GetGameInstance());
	if (!Time) return;

	Time->OnTimelineUpdated.AddDynamic(this, &UPlayerHUDUI::TimeUpdate); 
}

void UPlayerHUDUI::TimeUpdate(const FTimeData& TimeData)
{
	HourTextBlock->SetText(FText::AsNumber(TimeData.Hour));
	MinuteTextBlock->SetText(FText::AsNumber(TimeData.Minute % 10));
	TenthMinuteTextBlock->SetText(FText::AsNumber(TimeData.Minute / 10));
}
