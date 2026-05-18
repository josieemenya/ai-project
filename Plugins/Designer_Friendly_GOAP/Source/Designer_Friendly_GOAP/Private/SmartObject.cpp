// Fill out your copyright notice in the Description page of Project Settings.


#include "SmartObject.h"
#include "SmartObjectsManager.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


int ASmartObject::InstanceNumber = 0;

// Sets default values
ASmartObject::ASmartObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SeeObject = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("SeeObject");
	SeeObject->bAutoRegister = true;
	SeeObject->RegisterForSense(UAISense_Sight::StaticClass());
}

// Called when the game starts or when spawned
void ASmartObject::BeginPlay()
{
	Super::BeginPlay();
	InstanceNumber++;
	
	ObjectID = InstanceNumber;

	if (UWorld* World = GetWorld())
	{
	    if (auto GI = World->GetGameInstance()->GetSubsystem<USmartObjectsManager>())
	    {
	        GI->SmartObjects.Add(ObjectID, this);
	    }
	}
	
	if (SeeObject)
	{
		SeeObject->RegisterWithPerceptionSystem();
	}
	
}

// Called every frame
void ASmartObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


