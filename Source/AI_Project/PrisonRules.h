// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartObject.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PrisonRules.generated.h"

class UWorld;

UENUM(BlueprintType)
enum class EActionType : uint8
{
	FIGHTING, // self explanatory
	VISIBLYARMED, // also self explanatory
	STEALING, // like if they're looting from an inmate
	ROLLCALL,
	CONTRABAND, // for random search??
	STOLENKEY // not returning a key
};


UENUM(BlueprintType, meta = (Bitflags))
enum class EPrisonState : uint8
	// using bitmasking && left shift notation, because i feel like you could have both a lightsout and lockdown situation
{
	NONE = 0 UMETA(Hidden),
	LOCKDOWN = 1 << 0, // value = 1
	ROUTINE = 1 << 1, // value = 2 
	LIGHTSOUT = 1 << 2, // value = 4 
	TAKEOVER = 1 << 3 // value = 8
};

UCLASS(Blueprintable)
class URuleBreakSound : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<USoundWave>> RuleBreakSounds;
};

UENUM(BlueprintType)
enum class ESignalState : uint8
{
	ACTIVE,
	CONSUMED,
	IGNORED
}; 

USTRUCT(BlueprintType)
struct FSignalData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SignalLifeSpan;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActionType ActionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StimulusLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ACharacter>> InvolvedCharacters;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	
	

	bool operator==(const FSignalData& Other) const
	{
		return InvolvedCharacters == Other.InvolvedCharacters && ActionType == Other.ActionType;
	}
};
UCLASS()
class ASignal : public ASmartObject
{
	GENERATED_BODY()

public:

	ASignal();

	FSignalData SignalData;

protected:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	virtual void BeginPlay() override;
};

UCLASS()
class AI_PROJECT_API USignalManagement : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	TMap<TObjectPtr<ASignal>, ESignalState> SignalPool;
	const int POOL_SIZE = 25;
	
	UFUNCTION(BlueprintCallable)
	void ActivateSignal(FSignalData Signal, FVector Location);
	
	UFUNCTION(BlueprintCallable)
	void DeactivateSignal(FSignalData Signal);
	
	UFUNCTION(BlueprintCallable)
	void ClearSignalPool();
	
	int32 NextID; 
};
	

UCLASS(Config=Game, DefaultConfig)
class AI_PROJECT_API UDataContainerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config)
	TSubclassOf<URuleBreakSound> BrokenRules;

	EPrisonState CurrentState;
};




UCLASS(Blueprintable)
class AI_PROJECT_API UAnimImpactObject : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundWave*> HitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UAnimMontage*> Montages;

protected:
	ACharacter* GetResultingCharacter(USkeletalMeshComponent* MeshComponent);
	AActor* GetActorFromSphereTrace(USkeletalMeshComponent* MeshComponent, FHitResult& HitResult,
	                                TArray<AActor*>& ActorsToIgnore);
	void AdjustDamageAndRules(ACharacter* Instigator, ACharacter* OtherInstigator);
	void PlayDamageSound(FVector Location, AActor* HitActor);
	void PlayDamageAnim(AActor* HitActor, AActor* Instigator);

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                        float FrameDeltaTime) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
};
