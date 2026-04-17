// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonGuardComponent.h"

#include "AI_ProjectCharacter.h"
#include "Damage.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UPrisonGuardComponent::UPrisonGuardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UPrisonGuardComponent::TakeItems()
{
	if (!GetOwner()->IsA(AAI_ProjectCharacter::StaticClass())) return; 
	if (!StealAction) return;
	
	TArray<AActor*> Actors;
	GetOwner()->GetOverlappingActors(Actors, ACharacter::StaticClass());
	
	for (AActor* Actor : Actors)
	{
		if (Actor->Implements<UPrisonGuardComponent>())
		{
			auto CompRef = Cast<UPrisonGuardComponent>(Actor->GetComponentByClass(UPrisonGuardComponent::StaticClass()));
			CompRef->RemovePrisonItems(Cast<ACharacter>(GetOwner())); 
		}
	}
}

// Called when the game starts
void UPrisonGuardComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializePrisonItems(); 
}

void UPrisonGuardComponent::AddAndUpdatePrisonItems(TSubclassOf<AActor> AddedItemClass)
{
	FPrisonItemHolder NewItem  {
	AddedItemClass,
	nullptr,
	}; 
	
	PrisonItems.Add(NewItem);
	InitializePrisonItems();
}

void UPrisonGuardComponent::RemovePrisonItems(ACharacter* TargetCharacter)
{
	if (GetOwner()->Implements<UDamage>())
	{
		auto Ref = Cast<UDamage>(GetOwner()->GetComponentByClass(UDamage::StaticClass()));
		if (Ref->bMortis)
		{
			// have a selection function that is activated through ui
			if (auto TargetRef = Cast<UPrisonGuardComponent>(TargetCharacter->GetComponentByClass(UPrisonGuardComponent::StaticClass())))
			{
				for (auto Item : PrisonItems)
				{
					TargetRef->AddAndUpdatePrisonItems(Item.CorrespondingItem); 
				}
			}
		}
	}
}

void UPrisonGuardComponent::InitializePrisonItems()
{
	if (PrisonItems.IsEmpty()) return;
	
	for (auto Items : PrisonItems)
	{
		if (!Items.ActorRef)
		{
			Items.ActorRef = NewObject<AActor>(this, Items.CorrespondingItem); 
		}
	}
}


// Called every frame
void UPrisonGuardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

