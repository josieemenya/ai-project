// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"


USTRUCT(BlueprintType) 
struct FCraftingItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID; 
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"), BlueprintReadWrite)
	int32 Quantity;
	
	bool operator==(const FCraftingItemData& Item) const
	{
		return Item.ItemID == ItemID;
	}
	
};

USTRUCT(BlueprintType)
struct FItemRecipe : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCraftingItemData> Ingredients;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCraftingItemData Result;
	
};


UCLASS()
class AI_PROJECT_API UItem : public UActorComponent
{
	GENERATED_BODY()
public:
	UItem() = default; 
	UPROPERTY(EditAnywhere)
	FName Name;
	
	UPROPERTY(EditAnywhere)
	int32 Quantity;
	
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon; 
	
	UPROPERTY(EditAnywhere)
	AActor* CorrespondingItem;
	
	bool operator==(const UItem* Item) const 
	{
		return Item->Name == Name;
	}
	
	
private: 
	int32 MaxQuantity;
	
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// has an array of Items
	UPROPERTY(EditAnywhere)
	TArray<UItem*> HeldItems; 
	
	//void AddItem(UItem* Item);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
