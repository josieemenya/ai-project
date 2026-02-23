// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathways.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APathways::APathways()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VisiblePath = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisiblePath"));
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void APathways::BeginPlay()
{
	Super::BeginPlay();
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APathways::OnComponentBeginOverlap);
		UE_LOG(LogTemp, Warning, TEXT("Binded"));
	}

	OnComponentBeginOverlap(nullptr, GetOwner(), nullptr, 0, false, FHitResult());
}

// Called every frame
void APathways::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APathways::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("APathways::OnComponentBeginOverlap"));
	if (!OtherActor || OtherActor == this)
	{
		UE_LOG(LogTemp, Warning, TEXT("no actor available")); 
		return;
	} 

	UE_LOG(LogTemp, Warning, TEXT("OVERLAP WITH: %s"), *GetNameSafe(OtherActor));
	
	
		if (ConnectedRoom.GetStringLength() != 0)
		{
			UWorld* World = GetWorld();
			UGameplayStatics::OpenLevel(this, ConnectedRoom); 
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ConnectedRoom opened"));
		}
	
}
