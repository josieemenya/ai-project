// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Stat"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWeaponComponent::Equip()
{
	if (auto Character = Cast<ACharacter>(GetOwner()))
	{
		CurrentlyEquippedWeapon.WeaponMesh->AttachToComponent( // replace charcter with desired weapon 
			Character->GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform, 
			FName("HandEquipSocket")
			); 

	}
}

void UWeaponComponent::Aim()
{
	if (CurrentlyEquippedWeapon.WeaponType == EWeaponType::GUN)
	{
		FHitResult HIT;
		GetWorld()->LineTraceSingleByChannel(
				HIT,
				CurrentlyEquippedWeapon
			)
	}
}

void UWeaponComponent::Attack()
{
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

