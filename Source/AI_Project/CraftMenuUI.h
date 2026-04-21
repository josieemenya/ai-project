// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftHelperContainer.h"
#include "CraftingComponent.h"
#include "Blueprint/UserWidget.h"
#include "CraftMenuUI.generated.h"

/**
 * 
 */

class USizeBox;
class UScrollBox;
class UScaleBox;
class UBorder; 
class UTextBlock;
class UCanvasPanel;
class UBackgroundBlur; 

UCLASS()
class AI_PROJECT_API UCraftMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	
	
};

UCLASS()
class AI_PROJECT_API UCraftMenuToolTipBox : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	UScrollBox* ItemScrollBox;
};

UCLASS()
class AI_PROJECT_API UCraftMenuToolTip : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	UScaleBox* ScaleBox;
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	UBorder* Border;
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	UTextBlock* IngredientText; 
};

UCLASS()
class AI_PROJECT_API UCraftMenuItems : public UUserWidget
{
	GENERATED_BODY()
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemRecipe CraftingData; // ???
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* SizeBox;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* Border;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* CraftDataTable;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CraftableItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCraftMenuToolTipBox> ToolTipBoxClass; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UCraftMenuToolTipBox* ToolTipBox; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UCraftMenuToolTipBox* CurrentToolTipBox; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCraftMenuToolTip> ToolTipClass; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCraftMenuToolTip> ToolTip;
	
	UFUNCTION(BlueprintPure)
	FString ConstructDisplaySting(const FCraftingItemData& Data); 
	
protected:

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};

UCLASS()
class AI_PROJECT_API UCraftMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	USizeBox* SizeBox;
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	UBackgroundBlur* BackgroundBlur;
	
	UPROPERTY(EditAnywhere, meta =(BindWidget))
	UScrollBox* ScrollBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CraftUI")
	TArray<FCraftHelperContainer> Container; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CraftUI")
	TSubclassOf<UCraftMenuItems> ItemsClass;
	
	UFUNCTION(BlueprintNativeEvent)
	void PlayAnim(); 
	
protected:
	virtual void NativeConstruct() override;
};


