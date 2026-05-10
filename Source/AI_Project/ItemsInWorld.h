// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftingComponent.h"
#include "SmartObject.h"
#include "Inventory.h"
#include "ItemsInWorld.generated.h"



/**
 * 
 */
UCLASS()
class AI_PROJECT_API AItemsInWorld : public ASmartObject
{
	GENERATED_BODY()
	
	public:
	
	AItemsInWorld(); 
	
	//DECLARE_DELEGATE(FOnVisibilyChanged, );
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FInventoryItem InventoryItemReference; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FItemRecipe CraftingItemData; 
	
	// do i add item holdable settings?
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionSphere;
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	protected:
	
	virtual void BeginPlay() override;
};
