// Fill out your copyright notice in the Description page of Project Settings.


#include "SupportUI.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USupportUI::NativeConstruct()
{
	Super::NativeConstruct();
	StartButton->OnClicked.AddDynamic(this, &USupportUI::USupportUI::OnStartClicked); 
	EndButton->OnClicked.AddDynamic(this, &USupportUI::OnEndClicked);
	if (GetWorld())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void USupportUI::OnStartClicked()
{
	if (UWorld* LevelWorld = GetWorld())
	{
		UGameplayStatics::OpenLevel(LevelWorld, StartLevelName);
		UGameplayStatics::SetGamePaused(LevelWorld, false);
	}
}

void USupportUI::OnEndClicked()
{
	if (UWorld* LevelWorld = GetWorld())
	{
		APlayerController* PlayerController = LevelWorld->GetFirstPlayerController();
		EQuitPreference::Type QuitPreference = EQuitPreference::Background; 
		UKismetSystemLibrary::QuitGame(LevelWorld, PlayerController, QuitPreference, true); 
	}
}
