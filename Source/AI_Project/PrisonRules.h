// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

USTRUCT(BlueprintType)
struct FRuleContext 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // i think visible & read only make more sense
	ACharacter* OffendingCharacter; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* ReportingCharacter; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActionType ActionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPrisonManagementSystem* ManagementSystem; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location; // Location where the offending crime took place, I don't think I need that though

	bool operator==(const FRuleContext& Other) const
	{
		return OffendingCharacter == Other.OffendingCharacter && 
				ActionType == Other.ActionType;
	}
};

USTRUCT(BlueprintType)
struct FRuleContextMultiple : public FRuleContext // for if a rule break involves multiple people, like for say, a fight
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* OtherOffendingCharacter;
	
	bool operator==(const FRuleContext& Other) const
	{
		return OffendingCharacter == Other.OffendingCharacter &&
			   ActionType == Other.ActionType; // tolerance
	}
};

UCLASS(Blueprintable)
class AI_PROJECT_API URule : public UDataAsset
{
	GENERATED_BODY()

	
public:
	
	UFUNCTION(BlueprintNativeEvent, meta = (WorldContext = "WorldContextObject"))
	bool bIsRuleBroken(UObject* WorldContext, const FRuleContext &Context); 
	
	UFUNCTION(BlueprintNativeEvent)
	void EstablishRuleBreak(AAIController* ResultingController); // set flag in BlackBorad 
};

UENUM(BlueprintType, meta = (Bitflags))
enum class EPrisonState : uint8 // using bitmasking && left shift notation, because i feel like you could have both a lightsout and lockdown situation
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

UCLASS(Config=Game, DefaultConfig)
class AI_PROJECT_API UDataContainerSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Config)
	TArray<TSubclassOf<URule>> Rules;
	
	UPROPERTY(EditAnywhere, Config)
	TSubclassOf<URuleBreakSound> BrokenRules;
	
	EPrisonState CurrentState;
};




UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AI_PROJECT_API URuleContainer : public UActorComponent
{
	GENERATED_BODY()
	public:
	
	UPROPERTY(VisibleAnywhere)
	TArray<FRuleContext> Rules;
	
	UFUNCTION(BlueprintCallable)
	void AddBrokenRule(const FRuleContext& BrokenRule); 
	
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRuleBrokenDelegate);
	
	FRuleBrokenDelegate OnRuleBroken;
	
	

	UFUNCTION(BlueprintCallable)
	void DecideConsequence(); 
	
	
	protected:
	
	virtual void BeginPlay() override;

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
	AActor* GetActorFromSphereTrace(USkeletalMeshComponent* MeshComponent, FHitResult& HitResult, TArray<AActor*>& ActorsToIgnore); 
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