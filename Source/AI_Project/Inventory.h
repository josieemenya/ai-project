// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

#define MAX_ITEM_STACK_SIZE 64
#define MAX_INVENTORY_ITEMS 25

UENUM(BlueprintType)
enum class EItemType : uint8
{
	WEAPON,
	TOOL,
	WEARABLE,
	RESOURCE,
	DEVICE, 
	NONE UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FItemHoldable
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Actor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Socket; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;
};

USTRUCT()
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	//optional short and large description

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"), BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStackable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemHoldable HoldingSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Icon;
	

	bool operator==(const FInventoryItem& Item) const
	{
		return Item.ID == ID;
	}
};




// please move to actual item class later

UCLASS(Blueprintable)
class AI_PROJECT_API UItemUseData : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Use();
};



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
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
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> ItemsInInventory; 

	UPROPERTY(EditAnywhere)
	UDataTable* ItemDatabase;

	//UFUNCTION(BlueprintCallable)
	FInventoryItem* FindInInventory(FInventoryItem& SearchedItem); 
	
	UFUNCTION(BlueprintCallable)
	bool SpaceInInventory();

	UFUNCTION(BlueprintCallable)
	bool OnAddToInventory(FName ItemName);
	
	UFUNCTION(BlueprintCallable)
	bool OnAddRefToInventory(FInventoryItem& Reference);


	//void AddItem(UItem* Item);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
