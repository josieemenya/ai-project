// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PrisonManagementSystem.generated.h"

/**
 * 
 */

UENUM()
enum class EMyEnum : uint8
{
	VISIBLYARMED,
	
};

UCLASS(Blueprintable) // strech goal
class AI_PROJECT_API UPrisonUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class UCanvasPanel* CanvasPanel;
	
	//UPROPERTY(EditAnywhere, meta =(BindWidget))
	
	
};

UCLASS()
class AI_PROJECT_API UPrisonManagementSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	public:
	
	bool bLockdown; 
};
