// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "AIController.h"

// Sets default values for this component's properties
UTreeComponent::UTreeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTreeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USelector::StatusRun()
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, TEXT("Executing Selector"));
	for (auto child : children)
	{
		if (child->StatusRun())
			return true;
		
	}
	
	return false;
}

bool USequences::StatusRun()
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, TEXT("Executing Sequence"));
	for (auto child : children)
	{
		if (!child->StatusRun())
			return false; 
	}
	return true;
}

bool UConditionNode::StatusRun()
{
	if (FCondition)
		return true;
	return false;
}

bool UTreeAction::StatusRun()
{
	return UTreeNode::StatusRun();
}

bool UAnimationAction::StatusRun()
{
	if (AnimToPlay)
	{
		UAnimInstance* OwnerInstance = Target->GetMesh()->GetAnimInstance(); 
		if (OwnerInstance && OwnerInstance->Montage_IsPlaying(AnimToPlay))
		{
			OwnerInstance->Montage_Play(AnimToPlay);
			return true;
		} return false;
	} return false;
}

bool UMoveAction::StatusRun()
{
	if (!Condition->StatusRun()) return false;
	if (!Target) return false; 
	auto Lead = Target->GetController();
	if (!Lead) return false;
	auto AILead = Cast<AAIController>(Lead);
	if (!AILead) return false;
	
	AILead->MoveToLocation(TargetLocation, 10.f); 
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Black, TEXT("Moving Ai to:")); 
	return true;
}