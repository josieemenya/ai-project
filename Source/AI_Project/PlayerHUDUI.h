// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDUI.generated.h"

/**
 * 
 */
struct FTimeData;

UCLASS(Blueprintable)
class UStatBarUI : public UUserWidget
{
	GENERATED_BODY()
public : 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* PlayerStatBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* PlayerHealthBar;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* StaminaBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDamage* DamageComp; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APawn* OwningPawn;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
};

UCLASS()
class AI_PROJECT_API UPlayerHUDUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWrapBox* WrapBox;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* CanvasPanel;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* HorizontalBox;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HourTextBlock;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Elipsis;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TenthMinuteTextBlock;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MinuteTextBlock;
	
	UPROPERTY(EditAnywhere)
	class UFont* TimeFont; 
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void TimeUpdate(const FTimeData& TimeData);
	
};
