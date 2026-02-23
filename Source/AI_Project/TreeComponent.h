// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TreeComponent.generated.h"

class UAnimMontage;
class ACharacter;

UCLASS( ABSTRACT )
class UTreeNode : public UObject
{
	GENERATED_BODY()
public : 
	UTreeNode* parent;
	virtual bool StatusRun()
	{
		return true;
	}; 
};


UCLASS(ABSTRACT)
class UComposite : public UTreeNode
{
	GENERATED_BODY()
public:
	TArray<UTreeNode*> children; 
};

UCLASS(BlueprintType, Blueprintable)
class AI_PROJECT_API UConditionNode : public UTreeNode
{
	GENERATED_BODY()
public:
	bool FCondition;
	bool StatusRun() override; // always override
	virtual ~UConditionNode() = default;
};

UCLASS(BlueprintType, Blueprintable)
class AI_PROJECT_API UTreeAction : public UTreeNode // very customizable
{
	public:
	GENERATED_BODY()
	virtual bool StatusRun() override;
};

UCLASS(BlueprintType, Blueprintable)
class AI_PROJECT_API UAnimationAction : public UTreeNode
{
	GENERATED_BODY()
public : 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimToPlay; // i don
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* Target;
	
	UFUNCTION(BlueprintCallable)
	virtual bool StatusRun() override;
};

UCLASS(BlueprintType, Blueprintable)
class AI_PROJECT_API UMoveAction : public UTreeNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UConditionNode* Condition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* Target;
	
	UFUNCTION(BlueprintCallable)
	virtual bool StatusRun() override;
};

UCLASS(BlueprintType, Blueprintable)
class AI_PROJECT_API USelector : public UComposite
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	virtual bool StatusRun() override; 
};

UCLASS(BlueprintType, Blueprintable)
class AI_PROJECT_API USequences : public UComposite
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual bool StatusRun() override;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API UTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTreeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USelector* RootNode;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
