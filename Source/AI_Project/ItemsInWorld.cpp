// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemsInWorld.h"

#include "AI_ProjectCharacter.h"
#include "Components/SphereComponent.h"


AItemsInWorld::AItemsInWorld()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(35.f); 
}

void AItemsInWorld::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	if (OtherActor->IsA(AAI_ProjectCharacter::StaticClass()))
	{
		// Cast
		auto PlayerChar = Cast<AAI_ProjectCharacter>(OtherActor);
		if (PlayerChar)
		{
			PlayerChar->Inventory->OnAddRefToInventory(InventoryItemReference); 
			StaticMeshComp->SetVisibility(false);
			if (Destroy(true))
			{
				UE_LOG(LogTemp, Warning, TEXT("this actor has been destroyed"));
			}
		}
	}
}

void AItemsInWorld::BeginPlay()
{
	Super::BeginPlay();
	ASmartObject::BeginPlay();
	if (CollisionSphere)
	{
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemsInWorld::OnComponentBeginOverlap);
	}
}
