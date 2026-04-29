// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDUI.h"

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

	//HourTextBlock->AddBinding()
}
