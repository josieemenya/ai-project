// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PrisonGuardComponent.generated.h"

USTRUCT(BlueprintType)
struct FPrisonItemHolder
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> CorrespondingItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> ActorRef;
	
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UPrisonGuardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPrisonGuardComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPrisonItemHolder> PrisonItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* StealAction; 
	
	void TakeItems(); 
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void InitializePrisonItems();
	
	UFUNCTION(BlueprintCallable)
	void AddAndUpdatePrisonItems(TSubclassOf<AActor> AddedItemClass);
	
	UFUNCTION(BlueprintCallable)
	void RemovePrisonItems(ACharacter* TargetCharacter); 
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};




