// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack.h"
#include "GameFramework/Character.h"
#include "Animation/AnimationAsset.h"
#include "Animation/AnimMontage.h"

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

void UAttack::Attack()
{
	
	auto Mesh = Cast<ACharacter>(GetOwner())->GetMesh(); 
	if (Mesh)
	{
		if (!AttackMontages.IsEmpty())
		{
			TriggerAttackAnim(MontageIndex); 
		}
		if (AttackAnim && AttackMontages.IsEmpty())
			Mesh->PlayAnimation(AttackAnim, false); // anim notify
	}
	UE_LOG(LogTemp, Warning, TEXT("woo bam bam")); 
}

void UAttack::TriggerAttackAnim_Implementation(int32 Index)
{
	
}
