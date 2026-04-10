// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelContext.h"
#include "UIManagerSystem.generated.h"

/**
 * 
 */
UCLASS()
class AI_PROJECT_API UUIManagerSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	UPROPERTY()
	UUserWidget* ActiveWidget;
	
	public:
	
	DECLARE_MULTICAST_DELEGATE(FOnActiveWidgetChanged);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, TSubclassOf<UUserWidget>> UIAvailableWidgets;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ELevelMode, TObjectPtr<UUserWidget>> ActiveWidgetPool;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ELevelMode CurrentMode;
	
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetOrCreateWidget(ELevelMode Mode); 
	
	UFUNCTION(BlueprintCallable)
	void SetActiveWidget(); 
	
	UFUNCTION(BlueprintCallable)
	void RenderWidget(); 
	
	UFUNCTION(BlueprintPure)
	UUserWidget* GetActiveWidget();
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//UFUNCTION()
	FOnActiveWidgetChanged OnActiveWidgetChanged;
};
