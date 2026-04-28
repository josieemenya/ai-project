// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingComponent.h"

#include "IDetailTreeNode.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

FCraftingItemData::FCraftingItemData() // resolve this
{
	ItemID = NAME_None;
	Quantity = 1;
	/*
	DisplayName = FText::FromString("Item Name");
	ItemType = EItemType::NONE;
	StaticMesh = nullptr; 
	Icon = nullptr;
	bIsStackable = false;
	
	HoldingSettings = FItemHoldable(
			nullptr,
			nullptr,
			"", 
			FTransform()
		); 
	*/
	
}

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bCraftingMenuEnabled = false; 
	// ...
}

void UCraftingComponent::ToggleMenu()
{
	if (!bCraftingMenuEnabled)
	{
		CraftingMenuWidget->AddToViewport(); 
		UE_LOG(LogTemp, Warning, TEXT("Widget Ptr: %p"), CraftingMenuWidget);
		//CraftingMenuWidget->ClearAndRebuild();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		// make ui visible add to viewport
	} else
	{
		CraftingMenuWidget->RemoveFromParent();
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
	
	bCraftingMenuEnabled = !bCraftingMenuEnabled;
}


void UCraftingComponent::FindInInventory(const FCraftingItemData& ItemData, TArray<FCraftingItemData>& Items)
{
	for (FInventoryItem InventoryItem : InventoryRef->ItemsInInventory)
	{
		if (InventoryItem.ID == ItemData.ItemID)
		{
			FCraftingItemData Item;
			
			Item.ItemID = ItemData.ItemID;
			Item.Quantity = InventoryItem.Quantity; 
			Item.Icon = ItemData.Icon;
			
			Items.Add(Item);
		}
	}
	
}

int32 UCraftingComponent::SumOfQuantity(const TArray<FCraftingItemData>& Items)
{
	int32 Total = 0;
	for (const FCraftingItemData Item : Items)
	{
		Total += Item.Quantity;
	}
	
	return Total;
}

bool UCraftingComponent::CanCraftItem(FCraftingItemData& DesiredItem)
{
	
	FString ContextString = FString(); 
	FItemRecipe* DesiredRecipe = CraftingItemDatabase->FindRow<FItemRecipe>(DesiredItem.ItemID, ContextString, true);
	
	if (DesiredRecipe)
	{
		// travers desired ingredients, find all in inventory, if not in invrntory return false && if quantity not adequate
		
		for (const FCraftingItemData& Ingredient : DesiredRecipe->Ingredients)
		{
			TArray<FCraftingItemData> AllInInventory;
			FindInInventory(Ingredient, AllInInventory);
			
			if (AllInInventory.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("Inventory is empty"));
				return false;
			}
			
			if (SumOfQuantity(AllInInventory) < Ingredient.Quantity)
			{
				UE_LOG(LogTemp, Warning, TEXT("Not Enough Material"));
				return false;
			}
		}
	}	else
	{
		return false;
	}
	
	return true;
}

void UCraftingComponent::UpdateItemsInInventory(FItemRecipe* ItemData)
{
	for (auto Ingredient : ItemData->Ingredients)
	{
		
	}
}

bool UCraftingComponent::CraftItem(FCraftingItemData& DesiredItem)
{
	if (CanCraftItem(DesiredItem))
	{
		FString ContextString = FString();
		auto DesiredResult = CraftingItemDatabase->FindRow<FItemRecipe>(DesiredItem.ItemID, ContextString, true);
		if (DesiredResult)
		{
			auto InventoryItem = DesiredResult->Result; 
			UpdateItemsInInventory(DesiredResult); 
			return AddItemInInventory(InventoryItem); 
		}
	}
	
	return false;
}

bool UCraftingComponent::AddItemInInventory(FCraftingItemData& ItemData)
{
	FString ContextString;

	if (!InventoryRef)
	{
		GEngine->AddOnScreenDebugMessage(2, 1, FColor::Red, TEXT("InventoryRef is NULL"));
		return false;
	}

	if (!InventoryRef->ItemDatabase)
	{
		GEngine->AddOnScreenDebugMessage(2, 1, FColor::Red, TEXT("ItemDatabase is NULL"));
		return false;
	}

	auto SpecificItem = InventoryRef->ItemDatabase->FindRow<FInventoryItem>(ItemData.ItemID, ContextString, true);

	if (!SpecificItem)
	{
		GEngine->AddOnScreenDebugMessage(2, 1, FColor::Red, TEXT("Item not found in DataTable"));
		return false;
	}

	return InventoryRef->OnAddRefToInventory(*SpecificItem);
}

void UCraftingComponent::SortCraftableItems()
{
	CraftableItems.Sort([this](const auto a, const auto b)
		{
			int32 ScoreA = 0, ScoreB = 0;
			for (auto Item : AvailableItems)
			{
				if (a.Ingredients.Contains(Item))
				{
					ScoreA++; 
				}
				if (b.Ingredients.Contains(Item))
				{
					ScoreB++;
				}
			}
		
			return ScoreA > ScoreB; 
		}
	); 
}


TArray<FItemRecipe> UCraftingComponent::GetCraftableItems()
{
	if (CraftableItems.Num() > 0) {
		CraftableItems.Empty(); 
	}
	CraftingItemDatabase->GetAllRows<FItemRecipe>("", CraftableItems); 
	TArray<FItemRecipe> BP_CraftableItems;
	for (auto i : CraftableItems)
	{
		if (i)
		{
			BP_CraftableItems.Add(*i); 
		}
	}
	
	return BP_CraftableItems; 
}

// Called when the game starts
void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();
	GetCraftableItems();
	// sort most likely to craft (has partly relevant materials) 
	SortCraftableItems(); 
	// higlight the ones that you can craft, for now we might play a sound that you can't craft
	// ...
	CraftingMenuWidget = CreateWidget(GetWorld(), CraftingMenu);
}


// Called every frame
void UCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

