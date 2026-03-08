// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackboardSystem.generated.h"

class ASmartObject;

UENUM(BlueprintType)

enum class EBlackboardKey : uint8
{
	Int  UMETA(DisplayName = "int32"),
	Float UMETA(DisplayName = "float"),
	Double UMETA(DisplayName = "double"),
	Bool UMETA(DisplayName = "bool"),
	Vector UMETA(DisplayName = "FVector"),
	String UMETA(DisplayName = "FString"),
	Actor UMETA(DisplayName = "Actor"),
	SmartObject UMETA(DisplayName = "SmartObject"),
};

USTRUCT(BlueprintType)
struct FBlackboardCustomEntry
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EntryName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBlackboardKey ValueType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition="ValueType == EBlackboardKey::Int"))
	int32 IntValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition="ValueType == EBlackboardKey::Float"))
	float FloatValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition="ValueType == EBlackboardKey::Double"))
	double DoubleValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition="ValueType == EBlackboardKey::Bool"))
	bool BoolValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition="ValueType == EBlackboardKey::Vector"))
	FVector VectorValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition="ValueType == EBlackboardKey::String"))
	FString StringValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition="ValueType == EBlackboardKey::Actor"))
	AActor* ActorValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(EditCondition = "ValueType == EBlackboardKey::SmartObject"))
	ASmartObject* SmartObjectValue;
	
};


UCLASS(Blueprintable, BlueprintType)
class AI_PROJECT_API UBlackboardCustom : public UDataAsset
{
	GENERATED_BODY()
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBlackboardCustomEntry> BlackboardEntries;
};

UCLASS(Blueprintable, BlueprintType)
class AI_PROJECT_API UBlackboardSystem : public UObject
{
	GENERATED_BODY()
	
private:

	bool FindValue(const FBlackboardCustomEntry& Entry, const FName& KeyName) const
	{
		if (Entry.EntryName == KeyName)
			return true;
		return false;
	}

public:	
	// Sets default values for this component's properties
	UBlackboardSystem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlackboardCustom* Blackboard;
	
	UFUNCTION(BlueprintCallable)
	void Initialize(UBlackboardCustom* OtherBlackboard);
	
	//UFUNCTION(BlueprintCallable)
	FBlackboardCustomEntry* GetValue(FName KeyName) const;
	
	UFUNCTION(BlueprintCallable)
	void SetValueAsInt(FName KeyName, int32 Value); 
	
	UFUNCTION(BlueprintCallable)
	void SetValueAsFloat(FName KeyName, float Value);
	
	UFUNCTION(BlueprintCallable)
	void SetValueAsDouble(FName KeyName, double Value);
	
	UFUNCTION(BlueprintCallable)
	void SetValueAsBool(FName KeyName, bool Value);
	
	UFUNCTION(BlueprintCallable)
	void SetValueAsVector(FName KeyName, FVector Value);
	
	UFUNCTION(BlueprintCallable)
	void SetValueAsString(FName KeyName, FString Value);
	
	UFUNCTION(BlueprintCallable)
	void SetValueAsActor(FName KeyName, AActor* Value);
	
	UFUNCTION(BlueprintCallable)
	void SetValueAsSmartObject(FName KeyName, ASmartObject* Value);
	
	UFUNCTION(BlueprintPure)
	TArray<ASmartObject*> GetValueAllSmartObjects() const;
};