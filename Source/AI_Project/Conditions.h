// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Conditions.generated.h"

/**
 * 
 */
class AI_PROJECT_API Conditions
{
public:
	Conditions();
	~Conditions();
};

UCLASS(BlueprintType, Blueprintable)
class AI_PROJECT_API UCondition : public UDataAsset
{
	GENERATED_BODY()

public:
	UCondition(){};
	
	UFUNCTION(BlueprintNativeEvent)
	bool Evaluate(AActor* OwningCharacter) const;
	
	bool Evaluate_Implementation(AActor* OwningCharacter) const
	{
		return true;
	}
};

/*
USTRUCT(BlueprintType)
struct FFloatCondition : public FConditions
{
	GENERATED_BODY()
	
	//UFUNCTION(BlueprintCallable)
	FFloatCondition() : TestValue(0.f), minValue(0.f), maxValue(1.f)
	{
		
	}; 
	
	
	//UFUNCTION(BlueprintCallable)
	FFloatCondition(float testValue) : TestValue(testValue), minValue(0.f), maxValue(1.f)
	{
		
	};
	
	UFUNCTION(BlueprintCallable)
	void SetTestValue(float testValue)
	{
		TestValue = testValue;
	}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TestValue;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float minValue;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float maxValue;
	
	
	UFUNCTION(BlueprintCallable) 
	bool Evaluate() const
	{
		return TestValue >= minValue && TestValue <= maxValue;
	}; 
};


USTRUCT(BlueprintType)
struct FAndCondition : public FConditions
{
	GENERATED_BODY()
	
	//UFUNCTION(BlueprintCallable)
	FAndCondition() : CondtionA(nullptr), ConditionB(nullptr) {}
	
	//UFUNCTION(BlueprintCallable)
	FAndCondition(FConditions* A, FConditions* B) : CondtionA(A), ConditionB(B) {}
	
	UFUNCTION(BlueprintCallable)
	void SetFirstCondition(FConditions* testValue)
	{
		CondtionA = testValue;
	}
	
	UFUNCTION(BlueprintCallable)
	void SetSecondCondition(FConditions* testValue)
	{
		ConditionB = testValue;
	}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FConditions* CondtionA; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FConditions* ConditionB;
	
	UFUNCTION(BlueprintCallable)
	bool Evaluate() const
	{
		return CondtionA && ConditionB;
	}
};


USTRUCT(BlueprintType)
struct FOrCondition : public FConditions
{
	GENERATED_BODY()
	
	//UFUNCTION(BlueprintCallable)
	FOrCondition() : ConditionA(nullptr), ConditionB(nullptr) {}
	
	//UFUNCTION(BlueprintCallable)
	FOrCondition(FConditions* conditionA, FConditions* conditionB) : ConditionA(conditionA), ConditionB(conditionB) {}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FConditions* ConditionA;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FConditions* ConditionB;
	
	UFUNCTION(BlueprintCallable)
	bool Evaluate() const
	{
		return ConditionA->Evaluate() || ConditionB->Evaluate();
	}
};

USTRUCT(BlueprintType) struct FNotCondition : public FConditions
{
	FNotCondition() : Condition(nullptr)
	{
	}

	FNotCondition(FConditions* condition) : Condition(condition)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) 
	FConditions* Condition;
	
	UFUNCTION(BlueprintCallable) 
	bool Evaluate() const { return !Condition->Evaluate(); }
};
*/