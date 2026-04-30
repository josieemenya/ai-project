// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputAction.h"
#include "Attack.generated.h"

class UInputAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UAttack : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttack();
	
	UPROPERTY(EditAnywhere, Category = PlayerAttack)
	UInputAction* BaseAttack; 
	
	
	UPROPERTY(EditAnywhere, Category = AttackAnimations)
	TArray<UAnimMontage*> AttackMontages; 
	
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* TestMontage;
	
	UPROPERTY(EditAnywhere)
	TArray<USoundBase*> AttackSounds;  
	
	
	UPROPERTY(EditAnywhere, Category = PlayerAttack)
	UAnimationAsset* AttackAnim;
	
	UFUNCTION(BlueprintCallable)
	void Attack(); 

	
	UPROPERTY(BlueprintReadWrite)
	int32 MontageIndex; 
	
	UFUNCTION(BlueprintNativeEvent)
	void TriggerAttackAnim(int32 Index); 
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void OnEnemyHit(AActor* EnemyActor); 
		
};
