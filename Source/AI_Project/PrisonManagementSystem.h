// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrisonRules.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sound/SoundCue.h"
#include "Engine/DeveloperSettings.h"

#include "PrisonManagementSystem.generated.h"

/**
 * 
 */

class UTimeSystem;
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
	class USoundCue* LevelMusic;
	
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
class AI_PROJECT_API UPrisonManagementSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	
	void InitializeRulesFromSettings();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UPROPERTY(BlueprintReadWrite, meta =  (Bitmask, BitmaskEnum = "EPrisonState"))
	int32 PrisonStateFlags;
	
	UPROPERTY(BlueprintReadOnly)
	URuleBreakSound* BreakSounds; 	
	
	UFUNCTION(BlueprintCallable)
	void AddFlag(EPrisonState State); 
	
	UFUNCTION(BlueprintCallable)
	void RemoveFlag(EPrisonState State);
	
	void RemoveAllFlags();
	
	UPROPERTY(BlueprintReadWrite)
	bool bLockdown; 
	
	UFUNCTION(BlueprintCallable)
	void PrintPrisonState(); 
};

/*UCLASS()
class AI_PROJECT_API URollcallManagement : public UGameInstanceSubsystem
{
	
	GENERATED_BODY()
	
	UTimeSystem* Time; 
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};*/