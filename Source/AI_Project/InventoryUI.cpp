// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"

#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UInventoryItemUI::NativeConstruct()
{
	Super::NativeConstruct();
}

UInventoryItemUI* UInventoryUI::MakeItem(const FInventoryItem& InventoryItem)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UInventoryItemUI* ItemUI = Cast<UInventoryItemUI>(CreateWidget(PC, UInventoryItemUI::StaticClass())); 
	ItemUI->InventoryItemText->SetText(FText::FromName(InventoryItem.ID)); 
	ItemUI->InventoryQuantityText->SetText(FText::FromString(FString::FromInt(InventoryItem.Quantity)));
	ItemUI->Data = InventoryItem; 
	FSlateBrush Brush;
	Brush.SetResourceObject(InventoryItem.Icon); 
	ItemUI->InventoryItemImage->SetBrush(Brush);
	return ItemUI;
}

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	for (const FInventoryItem& InventoryItem : InventoryRef->ItemsInInventory)
	{
		ScrollBar->AddChild(MakeItem(InventoryItem));
	}
}
