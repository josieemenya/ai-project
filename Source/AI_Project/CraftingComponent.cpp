// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingComponent.h"
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
	FString ContextString;
	FItemRecipe* DesiredRecipe = CraftingItemDatabase->FindRow<FItemRecipe>(DesiredItem.ItemID, ContextString, true);

	if (!DesiredRecipe)
		return false;

	for (const FIngredient& RecipeVariant : DesiredRecipe->Recipes)
	{
		bool bCanCraftThisVariant = true;

		for (const FCraftingItemData& Ingredient : RecipeVariant.IngredientsForRecipe)
		{
			TArray<FCraftingItemData> AllInInventory;
			FindInInventory(Ingredient, AllInInventory);

			if (AllInInventory.IsEmpty() ||
				SumOfQuantity(AllInInventory) < Ingredient.Quantity)
			{
				bCanCraftThisVariant = false;
				break;
			}
		}

		if (bCanCraftThisVariant)
		{
			return true;
		}
	}

	return false;
}

void UCraftingComponent::UpdateItemsInInventory(FItemRecipe* ItemData)
{
	//for (auto Ingredient : ItemData->Ingredients)
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
	CraftableItems.Sort([this](const FItemRecipe& A, const FItemRecipe& B)
	{
		auto GetBestScore = [this](const FItemRecipe& Recipe)
		{
			int32 BestScore = 0;

			for (const FIngredient& Variant : Recipe.Recipes)
			{
				int32 Score = 0;

				for (const FCraftingItemData& Ingredient : Variant.IngredientsForRecipe)
				{
					for (const FCraftingItemData& Available : AvailableItems)
					{
						if (Ingredient.ItemID == Available.ItemID)
						{
							Score++;
							break;
						}
					}
				}

				BestScore = FMath::Max(BestScore, Score);
			}

			return BestScore;
		};

		int32 ScoreA = GetBestScore(A);
		int32 ScoreB = GetBestScore(B);

		return ScoreA > ScoreB;
	});
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

