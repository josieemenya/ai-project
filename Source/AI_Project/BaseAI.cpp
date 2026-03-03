// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAI.h"
//#include "LockAndKey.h"
#include "PlannerComponent.h"
#include "PlayerCharacterState.h"
#include "GameplayAbilitySpec.h"


// Sets default values
ABaseAI::ABaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlannerComponent = CreateDefaultSubobject<UPlannerComponent>("PlannerComponent");

}

// Called when the game starts or when spawned
void ABaseAI::BeginPlay()
{
	Super::BeginPlay();
	//Lock = new LockAndKey();
	
}

// Called every frame
void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateActions();

}

void ABaseAI::UpdateActions()
{
	PlannerComponent->UpdateStack(this); 
}

// Called to bind functionality to input
void ABaseAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ABaseAI::GetAbilitySystemComponent() const
{
	APlayerCharacterState* PS = Cast<APlayerCharacterState>(GetPlayerState());
	if (!IsValid(PS)) return nullptr;
	return PS->GetAbilitySystemComponent(); 
}


void ABaseAI::PossessedBy(AController* NewController)
{
	ACharacter::PossessedBy(NewController);
	if (!GetAbilitySystemComponent() || !HasAuthority()) return;
	
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	InitAbilities(); 
}

void ABaseAI::InitAbilities()
{
	if (!GetAbilitySystemComponent()) return;
}