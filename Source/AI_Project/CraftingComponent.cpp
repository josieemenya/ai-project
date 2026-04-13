// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingComponent.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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

	// ...
}

