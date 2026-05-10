// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"

#include "AI_ProjectCharacter.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "InputCoreTypes.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"


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



FReply UInventoryHotBarItem::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InKeyEvent)
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
	if (!InventoryRef) return;
	
	TArray<FInventoryItem> InventoryItems;
	
	for (int i = 0; i < MaxHotBarItems; i++)
	{
		if (InventoryRef->ItemsInInventory.IsValidIndex(i))
		{
			InventoryItems.Add(InventoryRef->ItemsInInventory[i]);
		}
	}
	
	for (FInventoryItem& InventoryItem : InventoryItems)
	{
		auto ItemHotBar = Cast<UInventoryHotBarItem>(CreateWidget(PC, InventoryHotBarItemClass)); 
		if (ItemHotBar)
		{
			ItemHotBar->ItemData = InventoryItem;
			ItemHotBar->UpdateUI();
			HorizontalBox->AddChild(ItemHotBar);
		}
	}
}





void UInventoryItemUI::NativeConstruct()
{
	Super::NativeConstruct();
	
}


void UInventoryItemUI::InitializeItem(const FInventoryItem& InItem)
{
	Data = InItem;

	if (!InventoryItemText || !InventoryQuantityText)
	{
		UE_LOG(LogTemp, Error, TEXT("Binding failed in InventoryItemUI"));
		return;
	}

	InventoryItemText->SetText(FText::FromName(Data.ID));
	InventoryQuantityText->SetText(FText::AsNumber(Data.Quantity));

	FSlateBrush Brush;
	if (InventoryItemImage)
	{
		Brush.SetResourceObject(Data.Icon);
		InventoryItemImage->SetBrush(Brush);
	}
}

FReply UInventoryItemUI::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FKey RMButton = EKeys::RightMouseButton;
	FKey LMButton = EKeys::LeftMouseButton;
	
	if (InMouseEvent.GetPressedButtons().Contains(LMButton))
	{
		if (TargetInventory)
		{
			TargetInventory->OnAddRefToInventory(Data); 
		}else
		{
			
		}
	}
	if (InMouseEvent.GetPressedButtons().Contains(RMButton))
	{
		GEngine->AddOnScreenDebugMessage(10,23.f, FColor::Yellow, TEXT("ClickedRMB"));
		
		// spawn item, deincreemtn by one
		// AActor
		FActorSpawnParameters SpawnParams;
		
		AAI_ProjectCharacter* PC = Cast<AAI_ProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		auto InventoryItemRef = PC->Inventory->FindInInventory(Data); 
		if (InventoryItemRef && InventoryItemRef->Quantity > 0)
		{
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
			FVector SpawnLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			AActor* Ref = GetWorld()->SpawnActor(AItem::StaticClass(), &SpawnLocation, &FRotator::ZeroRotator, SpawnParams); 
			
		
			AItem* ItemActor = Cast<AItem>(Ref);

			if (ItemActor)
			{
				ItemActor->StaticMesh->SetStaticMesh(Data.StaticMesh);
				ItemActor->StaticMesh->SetSimulatePhysics(true);
				ItemActor->StaticMesh->SetEnableGravity(true);

				ItemActor->ItemReference = Data;
				ItemActor->ItemReference.Quantity = 1;
			}
		
			InventoryItemRef->Quantity--;
			Data.Quantity--; 
			InventoryQuantityText->SetText(FText::AsNumber(Data.Quantity));
			
			if (Data.Quantity <= 0)
			{
				SetVisibility(ESlateVisibility::Collapsed);

				RemoveFromParent(); 
			}
		}
	}
	
	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

UInventoryItemUI* UInventoryUI::MakeItem(const FInventoryItem& InventoryItem)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	if (!PC || !InventoryItemClass)
	{
		return nullptr;
	}
	
	UInventoryItemUI* ItemUI = CreateWidget<UInventoryItemUI>(PC, InventoryItemClass);
	

	if (ItemUI)
	{
		ItemUI->InitializeItem(InventoryItem);
		ItemUI->SynchronizeProperties();
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("No ItemUI")); 
	} 
	
	return ItemUI;
}

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	bInventoryOpen = false; 
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ACharacter* PlayerChar = Cast<ACharacter>(PC->GetCharacter());
	
	if (PlayerChar)
	{
		InventoryRef = PlayerChar->FindComponentByClass<UInventory>();
		if (InventoryRef)
		{
			
			if (InventoryRef->ItemsInInventory.IsEmpty())
			{
				return;
			}
			
			ScrollBar->ClearChildren(); 
			
			for (const FInventoryItem& InventoryItem : InventoryRef->ItemsInInventory)
			{
				ScrollBar->AddChild(MakeItem(InventoryItem));
				//RefreshUI(); 
			}
			UE_LOG(LogTemp, Error, TEXT("Inventory."))
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not find Inventory Reference."))
		}
	}
}
