// Fill out your copyright notice in the Description page of Project Settings.


#include "BTController.h"
#include "TreeComponent.h"

ABTController::ABTController()
{
    Comp = CreateDefaultSubobject<UTreeComponent>(TEXT("TreeComp"));     
}
