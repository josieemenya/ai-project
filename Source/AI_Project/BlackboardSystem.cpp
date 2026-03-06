// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackboardSystem.h"
#include "SmartObject.h"

// Sets default values for this component's properties
UBlackboardSystem::UBlackboardSystem()
{
	
}


void UBlackboardSystem::Initialize(UBlackboardCustom* OtherBlackboard)
{
	//Blackboard = NewObject<UBlackboardCustom>(this);
	Blackboard = OtherBlackboard;
}

FBlackboardCustomEntry* UBlackboardSystem::GetValue(FName KeyName) const
{
	auto Find = [this, KeyName](const FBlackboardCustomEntry& Entry)
	{
		return FindValue(Entry, KeyName);
	}; 
	auto it = Blackboard->BlackboardEntries.FindByPredicate(Find);
	return it;
}

void UBlackboardSystem::SetValueAsInt(FName KeyName, int32 Value)
{
	if (GetValue(KeyName))
		GetValue(KeyName)->IntValue = Value;
}

void UBlackboardSystem::SetValueAsFloat(FName KeyName, float Value)
{
	if (GetValue(KeyName))
		GetValue(KeyName)->FloatValue = Value;
}

void UBlackboardSystem::SetValueAsDouble(FName KeyName, double Value)
{
	if (GetValue(KeyName))
		GetValue(KeyName)->DoubleValue = Value;
}

void UBlackboardSystem::SetValueAsBool(FName KeyName, bool Value)
{
	if (GetValue(KeyName))
		GetValue(KeyName)->BoolValue = Value;
}

void UBlackboardSystem::SetValueAsVector(FName KeyName, FVector Value)
{
	if (GetValue(KeyName))
		GetValue(KeyName)->VectorValue = Value;
}

void UBlackboardSystem::SetValueAsString(FName KeyName, FString Value)
{
	if (GetValue(KeyName))
		GetValue(KeyName)->StringValue = Value;
}

void UBlackboardSystem::SetValueAsActor(FName KeyName, AActor* Value)
{
	if (GetValue(KeyName))
		GetValue(KeyName)->ActorValue = Value;
}

void UBlackboardSystem::SetValueAsSmartObject(FName KeyName, ASmartObject* Value)
{
	if (GetValue(KeyName))
		GetValue(KeyName)->SmartObjectValue = Value;
}

TArray<ASmartObject*> UBlackboardSystem::GetValueAllSmartObjects() const 
{
	TArray<ASmartObject*> Result;
	for (const auto& Entry : Blackboard->BlackboardEntries)
	{
		if (Entry.ValueType == EBlackboardKey::SmartObject)
		{
			Result.Add(Entry.SmartObjectValue); 
		}
	}
	return Result;
}
