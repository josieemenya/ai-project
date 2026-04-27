// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

/**
 * 
 */

struct FInventoryItem;

UCLASS(Blueprintable) 
class AI_PROJECT_API UInventoryItemUI : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UWrapBox* WrapBox;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* InventoryItemImage; 
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* InventoryItemText;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UHorizontalBox* HorizontalBox;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* InventoryQuantityText;
	
	
protected:
	
	virtual void NativeConstruct() override;
	
};
 
 
UCLASS()
class AI_PROJECT_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UCanvasPanel* Canvas;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UHorizontalBox* HorizontalBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FInventoryItem> InventoryItems; 
	
	protected:
	
	virtual void NativeConstruct() override;
	
};
