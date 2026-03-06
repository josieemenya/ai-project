// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "SmartObjectManager.h"

EExitSequenceType UAction::Execute_Implementation(AActor* Owner)
{
	return EExitSequenceType::SUCCESS; 
}

TSoftObjectPtr<ASmartObject> UAction::GetSmartObject()
{
	TSoftObjectPtr<ASmartObject> FoundObj; 
	for (auto &Obj : GetWorld()->GetGameInstance<USmartObjectManager>()->SmartObjects)
	{
		auto Satisfying = [this](UAction* A)
		{
			return A->Effects.Satisfies(Context);
		}; 
		
		auto it =  Obj.Value->PossibleActions.FindByPredicate(Satisfying);
		
		if (it)
		{
			FoundObj = Obj.Value;
			return FoundObj;
		}
	}
	return TSoftObjectPtr<ASmartObject>(nullptr);
}
