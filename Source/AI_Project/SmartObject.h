// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Action.h"
#include "SmartObject.generated.h"

class UBlackboardCustom; 

UCLASS()
class AI_PROJECT_API ASmartObject : public AActor
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWorldState RepresentedState;
	
	UFUNCTION(BlueprintCallable)
	void WriteToWorldState(FWorldState& TargetState); 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartObjects)
	int32 ObjectID; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmartObjects)
	class UAIPerceptionStimuliSourceComponent* SeeObject;
	
	UFUNCTION(BlueprintCallable)
	void RegisterInBlackboard(UBlackboardCustom* BB); 
	
};

inline void ASmartObject::WriteToWorldState(FWorldState& TargetState)
{
    FString KeyString = ObjectName.ToString(); 

    if (TargetState.StateValues.Contains(KeyString))
    {
        TargetState.StateValues[KeyString] = true;
    }
    else
    {
        TargetState.StateValues.Add(KeyString, true);
    }
}

