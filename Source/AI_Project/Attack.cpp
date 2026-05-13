// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack.h"

#include "Damage.h"
#include "GameFramework/Character.h"
#include "Animation/AnimationAsset.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values for this component's properties
UAttack::UAttack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttack::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttack::OnEnemyHit(AActor* EnemyActor)
{
	if (Cast<ACharacter>(EnemyActor)) // to be replaced by a base enemy class
	{
		if (EnemyActor->Implements<UDamage>())
		{
			// then register damage
		}
	}
}

void UAttack::Attack()
{
	auto Mesh = Cast<ACharacter>(GetOwner())->GetMesh();
	if (Mesh)
	{
		if (!IsAttacking())
		{
			SetAttacking(true);
		
			int SoundsSize = AttackSounds.Num() - 1;
			int SoundIndex = FMath::RandRange(0, SoundsSize);
			USoundBase* RandSound = AttackSounds[SoundIndex];

			if (RandSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), RandSound, GetOwner()->GetActorLocation());
			}
		}
	}
}

void UAttack::TriggerAttackAnim_Implementation(int32 Index)
{
}
