// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.h"
#include "CraftingComponent.generated.h"


class UInputAction; 
class UInventory;

USTRUCT(BlueprintType)
struct FCraftingItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	FCraftingItemData(); 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"), BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Icon;

	bool operator==(const FCraftingItemData& Item) const
	{
		return Item.ItemID == ItemID;
	}
};

USTRUCT(BlueprintType)
struct FIngredient
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCraftingItemData> IngredientsForRecipe; 
};

USTRUCT(BlueprintType)
struct FItemRecipe : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIngredient> Recipes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCraftingItemData Result;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingComponent();
	
	UPROPERTY(EditAnywhere)
	TArray<FCraftingItemData> AvailableItems; 
	
	//UPROPERTY(EditAnywhere)
	TArray<FItemRecipe*> CraftableItems; 
	
	UPROPERTY(EditAnywhere)
	UDataTable* CraftingItemDatabase; 
	
	UFUNCTION(BlueprintCallable)
	TArray<FItemRecipe> GetCraftableItems(); 
	
	UPROPERTY(EditAnywhere)
	UInputAction* ToggleCraftingMenu; 
	
	UPROPERTY(EditAnywhere)
	bool bCraftingMenuEnabled;
	
	UFUNCTION(BlueprintCallable)
	void ToggleMenu(); 
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CraftingMenu;
	
	UUserWidget* CraftingMenuWidget;
	
	// inventory binding : 
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventory> InventoryClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventory* InventoryRef; 
	
	UFUNCTION(BlueprintCallable)
	bool CanCraftItem(FCraftingItemData& ItemData);
	
	UFUNCTION(BlueprintCallable)
	bool CraftItem(FCraftingItemData& DesiredItem); 
	
	UFUNCTION(BlueprintCallable)
	bool AddItemInInventory(FCraftingItemData& ItemData); 
	
	void FindInInventory(const FCraftingItemData& ItemData, TArray<FCraftingItemData>& Items);
	int32 SumOfQuantity(const TArray<FCraftingItemData>& Items); 
	
	void UpdateItemsInInventory(FItemRecipe* ItemData);

protected:
	void SortCraftableItems();
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
};
