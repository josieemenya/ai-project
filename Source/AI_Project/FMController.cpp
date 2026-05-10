// Fill out your copyright notice in the Description page of Project Settings.


#include "FMController.h"

AFMController::AFMController()
{
    Comp = CreateDefaultSubobject<UFSMComponent>(TEXT("FSM"));
}

void AFMController::BeginPlay()
{
    AAIController::BeginPlay();
}
