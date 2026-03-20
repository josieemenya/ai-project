// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Conditions.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExitSequence.h"
#include "FSMComponent.generated.h"




UCLASS(BlueprintType)
class AI_PROJECT_API UConsideration : public UDataAsset
{
	GENERATED_BODY()
	public:
	
		UConsideration(){};
	
		UFUNCTION(BlueprintNativeEvent)
		float Evaluate(); 
};

UCLASS(BlueprintType, Blueprintable)
class AI_PROJECT_API UFSMState : public UObject
{
	GENERATED_BODY()
	
public: 
	UFSMState(); 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UCondition>> Conditions;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UConsideration>> Considerations;
	
	UFUNCTION(BlueprintCallable)
	float Evaluate(); 
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	bool bCanRun;
	
	// "OnEnter", "OnRun", "OnExit"
	UFUNCTION(BlueprintNativeEvent)
	EExitSequenceType OnEnter(AActor* Owner);
	
	UFUNCTION(BlueprintNativeEvent)
	EExitSequenceType OnExit(AActor* Owner);
	
	UFUNCTION(BlueprintNativeEvent)
	EExitSequenceType OnRun(AActor* Owner);
	
	UFUNCTION(BlueprintCallable)
	void EvaluateConditions(); 
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSMComponent();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UFSMState>> States;
	
	TArray<UFSMState*> AllStates;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UFSMState* CurrentState;
	
	void InitStates(); 
	
	UFUNCTION(BlueprintCallable)
	void UpdateState(); 
	
	TMap<UFSMState*, float> FilterAvailableStates(TArray<UFSMState*> AvailableStates);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

UCLASS(BlueprintType, Blueprintable)
class AI_PROJECT_API UUtilityTree : public UObject
{
	GENERATED_BODY()
	
	public:
	
	UUtilityTree(){};
	
	static UUtilityTree* Instance; 
	
	UFUNCTION(BlueprintCallable)
	static UUtilityTree* Get();
	
	UFUNCTION(BlueprintCallable)
	void ScoreStates(TMap<UFSMState*, float> &States);
	
	
};

