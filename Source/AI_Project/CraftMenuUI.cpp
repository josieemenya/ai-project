// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftMenuUI.h"

#include "AI_ProjectCharacter.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Slate/SGameLayerManager.h"


FString UCraftMenuItems::ConstructDisplaySting(const FCraftingItemData& Data)
{
	auto ID = Data.ItemID; 
	FName Opening = FName(" (");
	auto Quantity = FName("%d",Data.Quantity); 
	FName Closing = FName(")");
	
	FString DisplayName;
	DisplayName.Append(ID.ToString());
	DisplayName.Append(Opening.ToString());
	DisplayName.Append(Quantity.ToString());
	DisplayName.Append(Closing.ToString());
	
	return DisplayName;
}

void UCraftMenuItems::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()); 
	
	ToolTipBox = Cast<UCraftMenuToolTipBox>(CreateWidget(GetWorld()->GetFirstPlayerController(), ToolTipBoxClass));
	
	if (ToolTipBox)
	{
		ToolTipBox->AddToViewport(0); 
		ToolTipBox->SetPositionInViewport( MousePos );
		
		for (const FCraftingItemData& CrafableItem : CraftingItems)
		{
			ToolTip = Cast<UCraftMenuToolTip>(CreateWidget(GetWorld()->GetFirstPlayerController(), ToolTipClass));
			ToolTip->IngredientText->SetText(FText::FromString(ConstructDisplaySting(CrafableItem)));
			ToolTipBox->ItemScrollBox->AddChild(ToolTip);
		}
	}
	
	CurrentToolTipBox = ToolTipBox; 

}

void UCraftMenuItems::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (CurrentToolTipBox)
	{
		CurrentToolTipBox->RemoveFromParent();
		CurrentToolTipBox = nullptr;
	}
}

void UCraftMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	AAI_ProjectCharacter* PlayerRef = Cast<AAI_ProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (PlayerRef)
	{
		// Get Crafting CompRef 
		UCraftingComponent* CraftingComp = Cast<UCraftingComponent>(PlayerRef->GetComponentByClass(UCraftingComponent::StaticClass()));
		
		if (CraftingComp)
		{
			auto CraftableItems = CraftingComp->GetCraftableItems();
			if (CraftableItems.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("No Craftable items found"));
				return;
			} 
			
			ScrollBox->ClearChildren();
			
			for (auto CraftableItem : CraftableItems)
			{
				Container.Add({CraftableItem.Result.ItemID, CraftableItem.Ingredients}); 
				UCraftMenuItems* NewCraftItem = Cast<UCraftMenuItems>(CreateWidget(GetWorld()->GetFirstPlayerController(), ItemsClass)); 
			
				if (NewCraftItem)
				{
					FString DisplayText = CraftableItem.Result.ItemID.ToString();
					NewCraftItem->CraftableItemName->SetText(FText::FromString(DisplayText));
					ScrollBox->AddChild(NewCraftItem);
				}
			}
		}
	}

}

