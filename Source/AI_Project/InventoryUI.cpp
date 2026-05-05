// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "InputCoreTypes.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void UInventoryHotBarItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryHotBarItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UInventoryHotBarItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryHotBarItem::UpdateUI()
{
	HotBarSlotText->SetText(FText::FromName(ItemData.ID));
	FSlateBrush Brush;
	Brush.SetResourceObject(ItemData.Icon);
	InventoryItemImage->SetBrush(Brush);
	QuantityText->SetText(FText::AsNumber(ItemData.Quantity));
}

FReply UInventoryHotBarItem::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
                                                            const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

void UInventoryHotBarItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	GEngine->AddOnScreenDebugMessage(123, 45.f, FColor::Magenta, TEXT("Drag Detected"));

	auto DD = Cast<UDragDropOP>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOP::StaticClass()));

	if (!DD)
	{
		return;
	}

	if (!DragVisual)
	{
		return;
	}

	auto CreatedVisual = CreateWidget<UDragVisualWidget>(GetWorld()->GetFirstPlayerController(), DragVisual);

	FSlateBrush Brush;
	Brush.SetResourceObject(ItemData.Icon);

	CreatedVisual->ItemIcon->SetBrush(Brush);

	DD->DefaultDragVisual = CreatedVisual;
	DD->ItemData = ItemData;
	DD->SourceWidget = this;

	OutOperation = DD;
}

bool UInventoryHotBarItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                        UDragDropOperation* InOperation)
{
	//return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	auto DD = Cast<UDragDropOP>(InOperation);
	if (!DD || !DD->SourceWidget)
	{
		return false;
	}


	UInventoryHotBarItem* Source = DD->SourceWidget;
	UInventoryHotBarItem* Target = this;

	if (Source == Target)
	{
		return false;
	}

	FInventoryItem Temp = Target->ItemData;
	Target->ItemData = Source->ItemData;
	Source->ItemData = Temp;

	Target->UpdateUI();
	Source->UpdateUI();

	return true;
}

void UInventoryHotBar::RefreshInventory()
{
	if (!InventoryRef || !HorizontalBox) return;

	HorizontalBox->ClearChildren();

	for (int i = 0; i < MaxHotBarItems; i++)
	{
		if (!InventoryRef->ItemsInInventory.IsValidIndex(i))
			continue;

		const FInventoryItem& Item = InventoryRef->ItemsInInventory[i];

		UInventoryHotBarItem* ItemWidget = MakeHotBarItem(Item); 
		
		UE_LOG(LogTemp, Warning, TEXT("item made")); 
		HorizontalBox->AddChild(ItemWidget);
	}
}

void UInventoryUI::RefreshInventory()
{
	if (!InventoryRef || !ScrollBar) return;

	ScrollBar->ClearChildren();

	for (const FInventoryItem& Item : InventoryRef->ItemsInInventory)
	{
		ScrollBar->AddChild(MakeItem(Item));
	}
}

UInventoryHotBarItem* UInventoryHotBar::MakeHotBarItem(const FInventoryItem& Data)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UInventoryHotBarItem* HotBarItem = Cast<UInventoryHotBarItem>(CreateWidget(PC, InventoryHotBarItemClass));

	HotBarItem->ItemData = Data;
	FSlateBrush Brush;
	Brush.SetResourceObject(Data.Icon);
	HotBarItem->InventoryItemImage->SetBrush(Brush);
	HotBarItem->QuantityText->SetText(FText::AsNumber(Data.Quantity));
	HotBarItem->UpdateUI();

	UE_LOG(LogTemp, Warning, TEXT("ItemWidget created: %s"), *GetNameSafe(HotBarItem));
	
	return HotBarItem;
}


FReply UInventoryHotBarItem::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
                                                            const FPointerEvent& InKeyEvent)
{
	if (InKeyEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InKeyEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InKeyEvent);
}

void UInventoryHotBar::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	ACharacter* PlayerChar = Cast<ACharacter>(PC->GetCharacter());
	if (!PlayerChar) return;

	InventoryRef = PlayerChar->FindComponentByClass<UInventory>();

	if (InventoryRef)
	{
		InventoryRef->InventoryRefreshed.AddDynamic(
			this, &UInventoryHotBar::RefreshInventory);
		RefreshInventory();
	}
}

void UInventoryHotBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}


void UInventoryItemUI::NativeConstruct()
{
	Super::NativeConstruct();
}


void UInventoryItemUI::InitializeItem(const FInventoryItem& InItem)
{
	Data = InItem;

	if (!InventoryItemText || !InventoryQuantityText || !InventoryItemImage)
	{
		UE_LOG(LogTemp, Error, TEXT("Binding failed in InventoryItemUI"));
		return;
	}

	InventoryItemText->SetText(FText::FromName(Data.ID));
	InventoryQuantityText->SetText(FText::AsNumber(Data.Quantity));

	FSlateBrush Brush;
	Brush.SetResourceObject(Data.Icon);
	InventoryItemImage->SetBrush(Brush);
}

UInventoryItemUI* UInventoryUI::MakeItem(const FInventoryItem& InventoryItem)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UInventoryItemUI* ItemUI = Cast<UInventoryItemUI>(CreateWidget(PC, InventoryItemClass));

	if (ItemUI)
	{
		ItemUI->InitializeItem(InventoryItem);
	}

	return ItemUI;
}

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryRef->InventoryRefreshed.AddDynamic(this, &UInventoryUI::RefreshInventory);


	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ACharacter* PlayerChar = Cast<ACharacter>(PC->GetCharacter());

	if (PlayerChar)
	{
		InventoryRef = PlayerChar->FindComponentByClass<UInventory>();
		if (InventoryRef)
		{
			for (const FInventoryItem& InventoryItem : InventoryRef->ItemsInInventory)
			{
				ScrollBar->AddChild(MakeItem(InventoryItem));
			}
			UE_LOG(LogTemp, Error, TEXT("Inventory."))
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not find Inventory Reference."))
		}
	}
}
