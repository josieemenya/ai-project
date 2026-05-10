// Fill out your copyright notice in the Description page of Project Settings.


#include "BTController.h"
#include "TreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ABTController::ABTController()
{
    Comp = CreateDefaultSubobject<UTreeComponent>(TEXT("TreeComp"));    
    BBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BB")); 
}
