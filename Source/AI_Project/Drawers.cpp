// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawers.h"

#include "Inventory.h"
#include "PrisonRules.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "InventoryUI.h"
#include "Components/ScrollBox.h"

ADrawers::ADrawers()
{
	DrawersMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrawersMesh"));
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	InventoryRef = CreateDefaultSubobject<UInventory>("Inventory");
	RootComponent = SphereComp;
}

void ADrawers::OnOverlapSphere(UPrimitiveComponent* OverlappingComp, AActor* OtherActor,
	UPrimitiveComponent* OtherOverlappedComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
	bIsOverlapped = true; 
	
	if (ACharacter* IsCharacter = Cast<ACharacter>(OtherActor))
	{
		InteractingCharacter = IsCharacter; 
	}
}


void ADrawers::BeginPlay()
{
	Super::BeginPlay();
	this->OnClicked.AddDynamic(this, &ADrawers::OnClickedDraw); 
	if (SphereComp)
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ADrawers::OnOverlapSphere); 
		SphereComp->OnComponentEndOverlap.AddDynamic(this, &ADrawers::OnEndOverlapSphere);
	}
	
	// to see if it works i'll populate inventory with random items; 
	
	FString ContextString = FString(); 
	TArray<FInventoryItem*> Items;
	
	if (ItemTable) // more aggressive defensive programming
		ItemTable->GetAllRows<FInventoryItem>(ContextString, Items); 
	
	
	
	const int32 NumItems = 4; 
	
	for (int i = 0; i < NumItems; i++)
	{
		if (ItemTable)
		{
			int RandomIndex = FMath::RandRange(0, Items.Num() - 1);
			if (InventoryRef) // defensive programming
			{
				InventoryRef->OnAddRefToInventory(*Items[RandomIndex]);
			}
		}
	}
	
	DrawerWidget = CreateWidget<UInventoryUI>(GetWorld(), DrawerWidgetClass);
}

void ADrawers::InitInventoryUI()
{
	DrawerWidget->ScrollBar->ClearChildren(); 
	
	for (const FInventoryItem& Item : DrawerWidget->InventoryRef->ItemsInInventory)
	{
		DrawerWidget->ScrollBar->AddChild(DrawerWidget->MakeItem(Item));
	}
}

void ADrawers::OnClickedDraw(AActor* ClickedActor, FKey ButtonPress)
{
	
	DrawerWidget->InventoryRef = InventoryRef;
	
	InitInventoryUI(); 
	
	DrawerWidget->AddToViewport(); 
	
	GEngine->AddOnScreenDebugMessage(100, 34.f, FColor::MakeRandomColor(), TEXT("CreatedWidget")); 
	
	for (auto Bar : DrawerWidget->ScrollBar->GetAllChildren())
	{
		if (UInventoryItemUI* ItemUI =  Cast<UInventoryItemUI>(Bar))
		{
			if (InteractingCharacter)
			{
				UInventory* CharacterInventory = Cast<UInventory>(InteractingCharacter->FindComponentByClass(UInventory::StaticClass()));
				ItemUI->TargetInventory = CharacterInventory;
			}
		}
	}
	
	if (bIsOverlapped && InteractingCharacter && InteractingCharacter != OwningCharacter)
	{
		FSignalData StealingData = FSignalData();
		StealingData.ActionType = EActionType::STEALING;
		StealingData.ID = 0; // overridden in global manager
		StealingData.InvolvedCharacters.Add(InteractingCharacter);
		StealingData.SignalLifeSpan = 5.f; 
		StealingData.StimulusLocation = InteractingCharacter->GetActorLocation();
		
		GetWorld()->GetGameInstance()->GetSubsystem<USignalManagement>()->ActivateSignal(StealingData, StealingData.StimulusLocation); //😀
	}
}

void ADrawers::OnEndOverlapSphere(UPrimitiveComponent* OverlappingComp, AActor* OtherActor,
	UPrimitiveComponent* OtherOverlappedComp, int32 OtherBodyIndex)
{
	bIsOverlapped = false;
	InteractingCharacter = nullptr;
	DrawerWidget->RemoveFromViewport();
}