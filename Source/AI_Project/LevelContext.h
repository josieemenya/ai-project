// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelContext.generated.h"

/**
 * 
 */


class UWidgetSwitcher; 
class UWrapBox; 
class UImage; 
class UButton; 
class USlider;
class UCanvasPanel;
class UEditableTextBox;

UENUM(BlueprintType) 
enum class ELevelMode : uint8
{
	MainMenu,
	LevelPicker,
	AICustom,
	Options
};

USTRUCT(BlueprintType)
struct FPlayerUILevelDetails
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	int32 EnemiesToSpawn; 
	
	UPROPERTY(BlueprintReadWrite)
	FName LevelToSpawnName; 
};

USTRUCT(BlueprintType)
struct FUIAiData
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FUILevelIcons
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentIndex {0}; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UTexture2D>> Icons;
};



UCLASS(Blueprintable)
class AI_PROJECT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton; 
	
	UPROPERTY(meta = (BindWidget))
	UButton* EndGameButton;
};

UCLASS(Blueprintable)
class AI_PROJECT_API UOptions : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	USlider* VolumeLevel; 
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BackToMenuButton;
};

UCLASS(Blueprintable)
class AI_PROJECT_API UAICustomizer : public UUserWidget
{
	
	GENERATED_BODY()
public:
	
	FPlayerUILevelDetails PlayerLevelDetails;
	
	UPROPERTY(meta = (BindWidget))
	USlider* EnemiesToSpawn; 
		
	UFUNCTION()
	void ChangeEnemyNumber(float Value); 
	
	protected:
	virtual void NativeConstruct() override;
	//virtual void NativePreConstruct() override;
	
	
	// have a list of availbale goals,
	// on click in the ui, addto an array of chosen goals,
	// have a copy  of a controller, add to goals then send it somewhere ig 
	// do the same with actions
}; 

UCLASS()
class AI_PROJECT_API ULevelContext : public UUserWidget
{
	GENERATED_BODY()
public : 
	FPlayerUILevelDetails PlayerLevelDetails;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUILevelIcons UILevelIcons;
	
	// TArray<TObjectPtr<UTexture2D>> LevelPictures; , thinkig of changing it to 3D
	
	UPROPERTY(meta = (BindWidget))
	UButton* LeftButton; 
	
	UPROPERTY(meta = (BindWidget))
	UButton* RightButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SelectButton; 
	
	UPROPERTY(meta = (BindWidget))
	UButton* BeginButton; 
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel; 
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* LeftButtonLabel; 
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* RightButtonLabel; 
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* SelectButtonLabel; 
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* BeginButtonLabel; 
	
	UPROPERTY(meta = (BindWidget))
	class UImage* CurrentIcon; 
	
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	
	friend class UAICustomizer;
	
	
	UFUNCTION()
	void OnLeftButtonClicked();
	
	UFUNCTION()
	void OnRightButtonClicked();
};

