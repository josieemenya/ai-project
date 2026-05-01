// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonRules.h"

#include "Damage.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties


ACharacter* UAnimImpactObject::GetResultingCharacter(USkeletalMeshComponent* MeshComponent)
{
	return nullptr;
}

AActor* UAnimImpactObject::GetActorFromSphereTrace(USkeletalMeshComponent* MeshComponent, FHitResult& HitResult, TArray<AActor*>& ActorsToIgnore)
{
	FVector SocketLocation = MeshComponent->GetSocketLocation(SocketName);
	AActor* HitActor;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), 
		SocketLocation, 
		SocketLocation + 10.f, 
		20.f, 
		TraceTypeQuery1, 
		false,
		ActorsToIgnore,
		EDrawDebugTrace::Persistent,
		HitResult,
		true,
		FLinearColor::MakeRandomColor()
		);	
	
	HitActor = HitResult.GetActor();
	
	if (HitActor)
	{
		PlayDamageSound(HitActor->GetActorLocation(), HitActor);
		PlayDamageAnim(HitActor, MeshComponent->GetOwner());
	}
	return HitActor;
}

void UAnimImpactObject::AdjustDamageAndRules(ACharacter* Instigator, ACharacter* OtherInstigator)
{
	UDamage* DamageComp = Cast<UDamage>(OtherInstigator->GetComponentByClass(UDamage::StaticClass()));
	if (DamageComp)
	{
		DamageComp->DamageHealth(10.f);
		// MAKE RuleBreak
				
		FRuleContextMultiple Fighting = FRuleContextMultiple();
		Fighting.OffendingCharacter = Instigator;
		Fighting.OtherOffendingCharacter = OtherInstigator;
		Fighting.ActionType = EActionType::FIGHTING; 
		Fighting.Location = Fighting.OffendingCharacter->GetActorLocation();
				
		// add to rule breaking class 
				
		URuleContainer* ContainerComp = Cast<URuleContainer>(Fighting.OffendingCharacter->GetComponentByClass(URuleContainer::StaticClass()));
				
		if (ContainerComp)
		{
			ContainerComp->Rules.Add(Fighting);
			// get the other one 
			URuleContainer* GetOtherContainer = Cast<URuleContainer>(Fighting.OtherOffendingCharacter->GetComponentByClass(URuleContainer::StaticClass()));
			if (GetOtherContainer)
			{
				GetOtherContainer->Rules.Add(Fighting);	
			}
		}
	}
}

void UAnimImpactObject::PlayDamageSound(FVector Location, AActor* HitActor)
{
	if (HitActor)
	{
		int RandomIndex = FMath::RandRange(0, HitSounds.Num() - 1);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),  HitSounds[RandomIndex], Location);
	}
}

void UAnimImpactObject::PlayDamageAnim(AActor* HitActor, AActor* Instigator)
{
	ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
	ACharacter* InstigatorCharacter = Cast<ACharacter>(Instigator);
	
	FVector ToInstigator = (Instigator->GetActorLocation() - HitActor->GetActorLocation()).GetSafeNormal();
	float Product = FVector::DotProduct(HitActor->GetActorForwardVector(), Instigator->GetActorForwardVector());
	if (UKismetMathLibrary::InRange_FloatFloat(Product, 0.5f,0.5f)) // change this, this is wrong😭
	{
		UAnimMontage* AnimToPlay = Montages["Front"]; 
		if (AnimToPlay)
		{
			if (HitCharacter)
			{
				HitCharacter->GetMesh()->GetAnimInstance()->Montage_Play(AnimToPlay);
			}
		}
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(Product, -0.5f, 0.4))
	{
		
	}else if (UKismetMathLibrary::InRange_FloatFloat(Product, -1.0f, -0.6f))
	{
		UAnimMontage* AnimToPlay = Montages["Back"];
		if (AnimToPlay)
		{
			if (HitCharacter)
			{
				HitCharacter->GetMesh()->GetAnimInstance()->Montage_Play(AnimToPlay);
			}
		}
	}
}

void UAnimImpactObject::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	ACharacter* ResultingCharacter = Cast<ACharacter>(MeshComp->GetOwner()); 
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	if (ResultingCharacter)
	{
		
		if (ACharacter* PrisonCharacter = Cast<ACharacter>(GetActorFromSphereTrace(MeshComp, Hit, ActorsToIgnore)))
		{
			UDamage* DamageComp = Cast<UDamage>(PrisonCharacter->GetComponentByClass(UDamage::StaticClass()));
			AdjustDamageAndRules(ResultingCharacter, PrisonCharacter);
		}
	}
}

void UAnimImpactObject::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimImpactObject::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}
