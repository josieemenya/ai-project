// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SupportUI.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API USupportUI : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* CanvasPanel; 
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* StartButton; 
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* EndButton;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* StartTextBlock;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* EndTextBlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StartLevelName; 
	
	protected:
	
	UFUNCTION()
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnStartClicked(); 
	
	UFUNCTION()
	void OnEndClicked();
};
