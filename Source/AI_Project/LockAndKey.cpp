// Fill out your copyright notice in the Description page of Project Settings.


#include "LockAndKey.h"
#include "BaseAI.h"

LockAndKey::LockAndKey()
{
}

LockAndKey::~LockAndKey()
{
}

void LockAndKey::SetActions() {
	
	LookForKey = {
		"LookForKey", 
		{},
		[]()
		{return false;}, 
		{},
		1
	};
	LookForKey.Context = new FPlannerWorldState(); 
	LookForKey.Effects = new FPlannerWorldState(); 
	
	LookForKey.Effects->StateValues.Add("HasKey", false);
	LookForKey.DoAction = []()
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, TEXT("Searching For Key")); 
			return true; // has no prerequisites; 
	} ; 
	
	
	FoundKey = {
		"FoundKey",
		{},
		[]()->bool{return false;},
		{}, 
		1
	};
	FoundKey.Context = new FPlannerWorldState();
	FoundKey.Effects = new FPlannerWorldState();
	FoundKey.Context->StateValues.Add("HasKey", false);
	FoundKey.Effects->StateValues.Add("HasKey", true);
	FoundKey.DoAction = [](){ GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::MakeRandomColor(), TEXT("FoundKey")); return true; };
	
	
	OpenDoor ={
		"OpenDoor",
		{},
		[]()->bool{return false;},
		{},
		1
	} ; 
	
	OpenDoor.Context = new FPlannerWorldState();
	OpenDoor.Effects = new FPlannerWorldState();
	OpenDoor.Effects->StateValues.Add("Unlock Door", true);
	OpenDoor.Context->StateValues.Add("HasKey", true);
	OpenDoor.DoAction = [](){ GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, TEXT("Opening Door")); return true; };
	
	
	LeaveRoom = {
		"LeaveRoom",
		{},
		[]()->bool{return false;},
		{},
		1
	};
	LeaveRoom.Context = new FPlannerWorldState();
	LeaveRoom.Effects = new FPlannerWorldState();
	
	LeaveRoom.Context->StateValues.Add("HasKey", true);
	LeaveRoom.Context->StateValues.Add("Unlock Door", true);
	LeaveRoom.Effects->StateValues.Add("Door Open", true);

	AvailableActions.Add(LookForKey);
	AvailableActions.Add(FoundKey);
	AvailableActions.Add(OpenDoor);
	AvailableActions.Add(LeaveRoom);

}

void LockAndKey::UpdateGoal(UPlannerComponent* AIPlanner)
{
	AIPlanner->PlanGoal(LookForKey.Context, &UnlockDoor.DesiredState); 
}

void LockAndKey::SetGoal()
{
	if (Owner)
	{
		FPlannerGoal Goal = {
			FString("Unlock locked Door"),
			{},
			1
		};
		
		Goal.DesiredState.StateValues.Add("Door Open", false);
		
		if (Owner->PlannerComponent->Goals.Contains(Goal))
		{
			return;
		} else
		{
			Owner->PlannerComponent->Goals.Add(Goal);
		}
		
	} else 
		return; 
}
