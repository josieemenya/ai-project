// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryUI.generated.h"

/**
 * 
 */

struct FInventoryItem;

UCLASS(Blueprintable)
class UDragDropOP : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FInventoryItem ItemData;

	UPROPERTY()
	class UInventoryHotBarItem* SourceWidget;
};

UCLASS()
class UDragVisualWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemIcon;
};

UCLASS(Blueprintable)
class AI_PROJECT_API UInventoryHotBarItem : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	FInventoryItem ItemData;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UWrapBox* ScaleBox;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* InventoryItemImage;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* QuantityText; 
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* HotBarSlotText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDragVisualWidget> DragVisual; 
	
	protected:
	
	virtual void NativeConstruct() override;

public:
	//virtual bool IsHovered() const override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	void UpdateUI();
protected:
	
	 
	virtual FReply
	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	virtual FReply
	NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};

UCLASS(Blueprintable)
class AI_PROJECT_API UInventoryHotBar : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	
	const int32 MaxHotBarItems = 6; 
	
	UPROPERTY(BlueprintReadWrite)
	class UInventory* InventoryRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInventoryHotBarItem> InventoryHotBarItemClass; 
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UCanvasPanel* Canvas;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UHorizontalBox* HorizontalBox;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* HoverSlotText;
	
protected:
	
	virtual void NativeConstruct() override;
	
};

// make an inventory tool tip ui

UCLASS(Blueprintable) 
class AI_PROJECT_API UInventoryItemUI : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInventoryItem Data; 
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UWrapBox* WrapBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UImage* InventoryItemImage; 
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* InventoryItemText;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UHorizontalBox* HorizontalBox;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* InventoryQuantityText;
	
	
protected:
	
	virtual void NativeConstruct() override;
public:
	void InitializeItem(const FInventoryItem& InItem);
};
 
 
UCLASS()
class AI_PROJECT_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventoryItemUI> InventoryItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventory* InventoryRef; 
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UCanvasPanel* Canvas;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UScrollBox* ScrollBar;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FInventoryItem> InventoryItems; */
	
	UFUNCTION()
	UInventoryItemUI* MakeItem(const FInventoryItem& InventoryItem); 
	
	protected:
	
	virtual void NativeConstruct() override;
	
};
