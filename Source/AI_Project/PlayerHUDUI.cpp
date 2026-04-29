// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDUI.h"

#include "TimeSystem.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"

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
