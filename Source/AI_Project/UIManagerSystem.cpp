// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSystem.h"

#include "Animation/AnimNode_TransitionPoseEvaluator.h"
#include "Kismet/GameplayStatics.h"

void UUIManagerSystem::SetActiveWidget()
{
	//ActiveWidget = GetOrCreateWidget(CurrentMode);
	OnActiveWidgetChanged.Broadcast();
}

UUserWidget* UUIManagerSystem::GetOrCreateWidget(ELevelMode Mode)
{
	auto it = ActiveWidgetPool.Find(Mode);
	switch (Mode)
	{
		case ELevelMode::AICustom:
			if (!it)
			{
				auto NewWidget = CreateWidget(GetWorld(), UIAvailableWidgets["AICustom"]); 
				ActiveWidgetPool.Add(Mode, NewWidget);
			} else
			{
				return ActiveWidgetPool[Mode];
			}
			break; 
		
		case ELevelMode::LevelPicker : 
			if (!it)
			{
				auto NewWidget = CreateWidget(GetWorld(), UIAvailableWidgets["LevelPicker"]); 
				ActiveWidgetPool.Add(Mode, NewWidget);
			} else
			{
				return ActiveWidgetPool[Mode];
			}
		break; 
		
		case ELevelMode::MainMenu : 
			if (!it)
			{
				auto NewWidget = CreateWidget(GetWorld(), UIAvailableWidgets["MainMenu"]); 
				ActiveWidgetPool.Add(Mode, NewWidget);
			} else
			{
				return ActiveWidgetPool[Mode];
			}
		break; 
		
		case ELevelMode::Options :
			if (!it)
			{
				auto NewWidget = CreateWidget(GetWorld(), UIAvailableWidgets["Options"]); 
				ActiveWidgetPool.Add(Mode, NewWidget);
			} else
			{
				return ActiveWidgetPool[Mode];
			}
		break; 
	}
	
	return nullptr;
}


void UUIManagerSystem::RenderWidget()
{
	if (ActiveWidget)
	{
		ActiveWidget->AddToViewport(); 
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Active Widget Null"));
	}
}

UUserWidget* UUIManagerSystem::GetActiveWidget()
{
	return ActiveWidget;
}

void UUIManagerSystem::Deinitialize()
{
	Super::Deinitialize();
}

void UUIManagerSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	//OnActiveWidgetChanged.AddUObject(this, &UUIManagerSystem::RenderWidget);
	CurrentMode = ELevelMode::MainMenu;
	//ActiveWidget = GetOrCreateWidget(CurrentMode);
}
