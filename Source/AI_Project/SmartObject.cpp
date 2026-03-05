// Fill out your copyright notice in the Description page of Project Settings.


#include "SmartObject.h"
#include "SmartObjectManager.h"



int ASmartObject::InstanceNumber = 0;

// Sets default values
ASmartObject::ASmartObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASmartObject::BeginPlay()
{
	Super::BeginPlay();
	InstanceNumber++;
	
	ObjectID = InstanceNumber;
	GetWorld()->GetGameInstance<USmartObjectManager>()->SmartObjects[ObjectID] = this; 
}

// Called every frame
void ASmartObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

