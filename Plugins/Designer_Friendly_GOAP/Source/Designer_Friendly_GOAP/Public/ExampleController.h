// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ExampleController.generated.h"

class UUtilityReasoner;
class UPlannerComponent;

UCLASS()
class DESIGNER_FRIENDLY_GOAP_API AExampleController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExampleController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BB");
	class UBlackboardComponent* BlackboardComponent; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP System")
	TObjectPtr<UUtilityReasoner> Reasoner; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Planner Component")
	TObjectPtr<UPlannerComponent> Planner; 
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
