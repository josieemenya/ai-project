// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DeveloperSettings.h"
#include "PrisonManagementSystem.generated.h"

/**
 * 
 */

class URule; 
class UBackgroundBlur;

UENUM()
enum class EOptionValueType : uint8
{
	VOLUME,
	NONE
};



UCLASS(Blueprintable) // strech goal
class AI_PROJECT_API UPrisonUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class UCanvasPanel* CanvasPanel;
	

	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAudioComponent* AudioComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* LevelMusic;
	
	void UpdateLevelMusicVolume(float Val);
	
	protected:
	
	
	
	virtual void NativeConstruct() override;
	
};

UCLASS ()
class UOptionItem : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EOptionValueType OptionType;
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class UWrapBox* Wrapper; 
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class UTextBlock* PropertyName;
};

UCLASS()
class UOptionItemSlider : public UOptionItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class USlider* SliderVariable;
	
	UPrisonUI* PrisonRef; 
	
	UFUNCTION()
	void UpdateVolume(float Val); // ?? 
	
	void NativeConstruct() override;
};

UCLASS()
class UOptionItemInput : public UOptionItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class UEditableTextBox* InputBox;
};

UCLASS()
class UOptionsUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class UScaleBox* ScaleBox;
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	UBackgroundBlur* BackgroundBlur;
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	class UVerticalBox* VerticalBox;
};


UCLASS()
class AI_PROJECT_API UPrisonManagementSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	
	void InitializeRulesFromSettings();
	
	TArray<URule*> AllRules;
	
	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	int32 PrisonStateFlags; 
	
	//void OnRuleBreakOccured(); 
	
	bool bLockdown; 
};
