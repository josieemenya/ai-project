// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartObject.h"
#include "Drawers.generated.h"

/**
 * 
 */

class UInventoryUI;
class UInventory;
class ACharacter;

UCLASS()
class AI_PROJECT_API ADrawers : public ASmartObject
{
	GENERATED_BODY()
	
	public:
	ADrawers(); 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventory* InventoryRef; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* OwningCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ACharacter* InteractingCharacter; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* DrawersMesh; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UDataTable* ItemTable; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USphereComponent* SphereComp; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsOverlapped;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInventoryUI> DrawerWidgetClass; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryUI* DrawerWidget;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bNoRefill; 
	
	// add a ref for inventory UI
	
	UFUNCTION()
	void OnOverlapSphere(UPrimitiveComponent* OverlappingComp, AActor* OtherActor, UPrimitiveComponent* OtherOverlappedComp,  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep); 
	
	UFUNCTION()
	void OnClickedDraw(AActor* ClickedActor, FKey ButtonPress);
	
	UFUNCTION()
	void OnEndOverlapSphere(UPrimitiveComponent* OverlappingComp, AActor* OtherActor, UPrimitiveComponent* OtherOverlappedComp,  int32 OtherBodyIndex);
protected:
	virtual void BeginPlay() override;
	void InitInventoryUI(); 
};
