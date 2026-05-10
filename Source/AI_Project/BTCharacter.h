// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BTCharacter.generated.h"

class ABTController;
UCLASS()
class AI_PROJECT_API ABTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABTCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABTController> ControllerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABTController* BTController;

};
