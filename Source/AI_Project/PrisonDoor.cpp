// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonDoor.h"

#include "AI_ProjectCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APrisonDoor::APrisonDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APrisonDoor::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &APrisonDoor::OnClickDoor); 
	if (Capsule)
	{
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &APrisonDoor::OnOverlapCapsule); 
	}
	
	bIsOpen = false; 
}

void APrisonDoor::OnOverlapCapsule(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void APrisonDoor::OnClickDoor(AActor* TouchedActor, FKey Key)
{
	UE_LOG(LogTemp, Warning, TEXT("InteractingPlayerCharacter clicked"));

	if (Key == EKeys::LeftMouseButton)
	{
		if (bIsOpen)
		{
			if (UnlockDoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(UnlockDoorSound), GetActorLocation());
			}
			if (InteractingPlayerCharacter)
			{
				InteractingPlayerCharacter->SetActorLocation(GetActorLocation() + 20.f);
			}
			FVector NewLocation = GetActorLocation();
			NewLocation.Y -= 200; 
			UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->SetActorLocation(NewLocation); 
		} else
		{
			LockDoor();
		}
	}
}

void APrisonDoor::LockDoor()
{
	bIsOpen = false; 
	if (LockDoorSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(LockDoorSound), GetActorLocation());
	}
}

// Called every frame
void APrisonDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrisonDoor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (Other == this) return;
	
	InteractingPlayerCharacter = Cast<AAI_ProjectCharacter>(Other);
	UInventory* Inventory = nullptr;
	if (InteractingPlayerCharacter)
	{
		Inventory = InteractingPlayerCharacter->Inventory;
		if (Inventory)
		{
			if (auto Key = Inventory->FindItemNameInInventory(DoorKeyName))
			{
				bIsOpen = true; 
			}
		}
	}
}

