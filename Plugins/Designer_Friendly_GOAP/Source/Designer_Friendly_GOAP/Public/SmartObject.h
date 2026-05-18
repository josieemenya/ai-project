// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Action.h"
#include "SmartObject.generated.h"


UCLASS()
class DESIGNER_FRIENDLY_GOAP_API ASmartObject : public AActor
{
	GENERATED_BODY()
	
	static int InstanceNumber; 
public:	
	// Sets default values for this actor's properties
	ASmartObject();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UAction* DesiredActionIndex; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartObjects)
	FName ObjectName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmartObjects)
	FWorldState RepresentedState;
	
	UFUNCTION(BlueprintCallable, Category = SmartObjects)
	void WriteToWorldState(FWorldState& TargetState); 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartObjects)
	int32 ObjectID; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmartObjects)
	class UAIPerceptionStimuliSourceComponent* SeeObject;

	
};

inline void ASmartObject::WriteToWorldState(FWorldState& TargetState)
{
	UE_LOG(LogTemp, Warning, TEXT("Writing %s to StateValues"), *ObjectName.ToString());
	auto AlreadyFound = TargetState.StateValues.Find(ObjectName.ToString()); 
	if (!AlreadyFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Map count before (Adding): %d"), TargetState.StateValues.Num());
		TargetState.StateValues.Add(ObjectName.ToString(), true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Map count before(Updating): %d"), TargetState.StateValues.Num());
		TargetState.StateValues[ObjectName.ToString()] = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Map count after: %d"), TargetState.StateValues.Num());

}


UCLASS(Blueprintable)
class DESIGNER_FRIENDLY_GOAP_API USmartObjectsContainer : public UObject
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartObjects)
	TArray<ASmartObject*> RegisteredObjects;
};