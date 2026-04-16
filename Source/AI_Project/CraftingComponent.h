// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.h"
#include "CraftingComponent.generated.h"

struct FItemRecipe;
struct FCraftingItem; 
class UInputAction; 

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

protected:
	void SortCraftableItems();
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
};
