// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

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
	RootNode = NewObject<UTreeNode>(this, RootNodeClass);
	if (RootNode)
	{
		InitializeNode(RootNode);
	}
}

void UTreeComponent::InitializeNode(UTreeNode* Node)
{
	if (UComposite* Composite = Cast<UComposite>(Node))
	{
		for (auto ChildClass : Composite->ChildrenClasses)
		{
			UTreeNode* Child = NewObject<UTreeNode>(Composite, ChildClass);

			Child->Parent = Composite;
			Composite->InstancedChildren.Add(Child);

			InitializeNode(Child);
		}
	}
}


// Called every frame
void UTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RootNode)
		RootNode->StatusRun();
	// ...
}

EExitSequenceType USelector::StatusRun()
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, TEXT("Executing Selector"));
	for (auto child : InstancedChildren)
	{
		if (child && (child->StatusRun() == EExitSequenceType::SUCCESS))
		{
			return EExitSequenceType::SUCCESS;
		}
	}

	return EExitSequenceType::FAILURE;
}

EExitSequenceType USequences::StatusRun()
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, TEXT("Executing Sequence"));
	for (auto child : InstancedChildren)
	{
		if (!child) return EExitSequenceType::FAILURE;

		return child->StatusRun();
	}
	return EExitSequenceType::SUCCESS;
}

EExitSequenceType UConditionNode::StatusRun()
{
	if (FCondition)
		return EExitSequenceType::SUCCESS;
	return EExitSequenceType::FAILURE;
}

EExitSequenceType UTreeAction::StatusRun_Implementation()
{
	return EExitSequenceType::DEFAULT;
}

EExitSequenceType UAnimationAction::StatusRun()
{
	if (AnimToPlay)
	{
		UAnimInstance* OwnerInstance = Target->GetMesh()->GetAnimInstance();
		if (OwnerInstance)
		{
			if (!OwnerInstance->Montage_IsPlaying(AnimToPlay))
			{
				OwnerInstance->Montage_Play(AnimToPlay);
				return EExitSequenceType::RUNNING; // sucees
			}
			return EExitSequenceType::RUNNING;
		}
		return EExitSequenceType::FAILURE; // fail
	}
	return EExitSequenceType::FAILURE; // fail
}

EExitSequenceType UMoveAction::StatusRun()
{
	if (Condition->StatusRun() != EExitSequenceType::SUCCESS) return EExitSequenceType::FAILURE;
	if (!Target) return EExitSequenceType::FAILURE;
	auto Lead = Target->GetController();
	if (!Lead) return EExitSequenceType::FAILURE;
	auto AILead = Cast<AAIController>(Lead);
	if (!AILead) return EExitSequenceType::FAILURE;

	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Black, TEXT("Moving Ai to:"));
	
	switch (AILead->MoveToLocation(TargetLocation, 10.f))
	{
		case EPathFollowingRequestResult::AlreadyAtGoal:
			return EExitSequenceType::SUCCESS;
		
		case EPathFollowingRequestResult::RequestSuccessful:
			return EExitSequenceType::RUNNING;
		
		case EPathFollowingRequestResult::Failed:
			return EExitSequenceType::FAILURE;
	}
	
	return EExitSequenceType::FAILURE;
}
