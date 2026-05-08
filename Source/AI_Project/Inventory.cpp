// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "AI_ProjectCharacter.h"
#include "AudioMixerBlueprintLibrary.h"
#include "IDetailTreeNode.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#

// ad this to presentations

AItem::AItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	StaticMesh->SetupAttachment(GetRootComponent());
}

void AItem::Clicked(AActor* TouchedActor, FKey Key)
{
	if (Key == EKeys::LeftMouseButton)
	{
		AAI_ProjectCharacter* Character = Cast<AAI_ProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		if (Character)
		{
			Character->Inventory->OnAddRefToInventory(ItemReference); 
		}
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &AItem::Clicked); 
}

// Sets default values for this component's properties
UInventory::UInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


FInventoryItem* UInventory::FindInInventory(FInventoryItem& SearchedItem)
{
	FInventoryItem EmptyItem;
	for (auto &Item : ItemsInInventory)
	{
		if (SearchedItem == Item)
		{
			return &Item;
		}
	}
	
	return nullptr; 
}

bool UInventory::SpaceInInventory()
{
	return ItemsInInventory.Num() < MAX_INVENTORY_ITEMS;
}

bool UInventory::OnAddToInventory(FName ItemName)
{
	if (ItemName.IsNone()) return false;
	
	
	FString ContextString = FString(); 

	auto ItemRef = ItemDatabase->FindRow<FInventoryItem>(ItemName, ContextString, true); 

	if (auto InventoryItemRef = FindInInventory(*ItemRef))
	{
		if (InventoryItemRef->Quantity < MAX_INVENTORY_ITEMS)
		{
			InventoryItemRef->Quantity++;
			return true;
		}
	}
	
	if (SpaceInInventory())
	{
		FInventoryItem NewItem = *ItemRef;
		NewItem.Quantity = 1;
		ItemsInInventory.Add(*ItemRef);
		return true;
	}

	return false;

}

bool UInventory::OnAddRefToInventory(FInventoryItem& Reference)
{
	// this is worng
	if (auto HasItems = FindInInventory(Reference))
	{
		HasItems->Quantity++;
		return true;
	}
	
	if (SpaceInInventory())
	{
		ItemsInInventory.Add(Reference);
		return true;
	}
	return false;
}


void UItemUseData::Use_Implementation()
{
}


// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/*void UInventory::AddItem(UItem* Item)
{
	if (HeldItems.Contains(Item->Name))
	{
		auto* it = HeldItems.FindByPredicate([&](const UItem* Other)
		{
			return Item && Other && (Item->Name == Other->Name); 
		});
		
		if (it)
			++(*it)->Quantity; 
	} else
	{
		HeldItems.Add(Item); // please come back to this
	}
}*/
