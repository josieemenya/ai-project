// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "SmartObjectsManager.h"

EExitSequenceType UAction::Execute_Implementation(AActor* Owner)
{
	return EExitSequenceType::RUNNING; 
}
