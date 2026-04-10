// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContext.h"

#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components//EditableTextBox.h"
#include "Components/Image.h"


void ULevelContext::NativeConstruct()
{
	Super::NativeConstruct();
	
	LeftButton->OnClicked.AddUniqueDynamic(this, &ULevelContext::OnLeftButtonClicked);
	RightButton->OnClicked.AddUniqueDynamic(this, &ULevelContext::OnRightButtonClicked);
	LeftButtonLabel->SetText(FText::FromString("<-"));
	RightButtonLabel->SetText(FText::FromString("->"));
	SelectButtonLabel->SetText(FText::FromString("Select"));
	BeginButtonLabel->SetText(FText::FromString("Begin"));
	
	LeftButton->AddChild(LeftButtonLabel);
}

void ULevelContext::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void ULevelContext::OnLeftButtonClicked()
{
	if (UILevelIcons.CurrentIndex > 0)
	{
		UILevelIcons.CurrentIndex--;
		CurrentIcon->SetBrushFromTexture( 
			UILevelIcons.Icons[UILevelIcons.CurrentIndex]
			);
	} else
	{
		// make a popup or some shit
	}
}

void ULevelContext::OnRightButtonClicked()
{
	if (UILevelIcons.CurrentIndex < UILevelIcons.Icons.Num() - 1)
	{
		UILevelIcons.CurrentIndex++;
		CurrentIcon->SetBrushFromTexture( 
			UILevelIcons.Icons[UILevelIcons.CurrentIndex]
			);
	} else
	{
		// make a popup or some shit
	}
}

/* -------------------------------------------------------------------------- */
/*                                AI Customizer                               */
/* -------------------------------------------------------------------------- */



void UAICustomizer::ChangeEnemyNumber(float Value)
{
	int NewValue = FMath::RoundToInt(Value);
	PlayerLevelDetails.EnemiesToSpawn = NewValue;
}

void UAICustomizer::NativeConstruct()
{
	Super::NativeConstruct();
	EnemiesToSpawn->OnValueChanged.AddUniqueDynamic(this, &UAICustomizer::ChangeEnemyNumber); 
}

