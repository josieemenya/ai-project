// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAmmo.h"

#include "Damage.h"
#include "GPController.h"
#include "PrisonRules.h"
#include "GameFramework//Character.h"
#include "Kismet/GameplayStatics.h"


void URangedAmmoPool::Deinitialize()
{
	Super::Deinitialize();
}

void URangedAmmoPool::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Ammo.Reserve(MaxPoolSize);
	
	for (int i = 0; i < MaxPoolSize; i++)
	{
		if (UWorld* World = GetWorld())
		{
			AActor* NewAmmo = World->SpawnActor(ARangedAmmo::StaticClass()); 
			Ammo.Add(NewAmmo);
			NewAmmo->SetActorHiddenInGame(true);
			NewAmmo->SetActorEnableCollision(false);
			NewAmmo->SetActorTickEnabled(false);
		}
	}
}

// Sets default values
ARangedAmmo::ARangedAmmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARangedAmmo::BeginPlay()
{
	Super::BeginPlay();
	//AmmoMesh->OnComponentHit.AddDynamic(this, &ARangedAmmo::InjuredCombatant); 
	
}

// Called every frame
void ARangedAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARangedAmmo::InjuredCombatant(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* PrisonGuard = Cast<ACharacter>(OtherActor); 
	if (PrisonGuard)
	{
	}
	
	if (UDamage* GetComp = Cast<UDamage>(PrisonGuard->GetComponentByClass(UDamage::StaticClass())))
	{
		GetComp->DamageHealth(33.f);
		// Play electrocuteed montage
	}
}

