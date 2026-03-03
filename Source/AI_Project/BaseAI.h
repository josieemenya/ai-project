// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlannerComponent.h"
#include "BaseAI.generated.h"

class LockAndKey; 

class UPlannerComponent;

UCLASS()
class AI_PROJECT_API ABaseAI : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAI();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlannerComponent* PlannerComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	//LockAndKey* Lock; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateActions(); // called every tick to update the action stack, if the stack is empty, call the planner to generate a new plan based on the current world state and the desired goal state, then execute the first action in the stack and remove it from the stack

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void InitAbilities(); 
};
