// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeSystem.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"

UTimeSystem::UTimeSystem()
{
	DayPositions.FindOrAdd("Morning", -45.f);
	DayPositions.Add("Evening", -10.f);
	DayPositions.Add("Afternoon", -135.f);
	DayPositions.Add("Twilight", 0);
}

void UTimeSystem::TransitionTimeOfDay(ETimeOfDay e)
{
	float TimeLength = 7.0f; 
	
	
	switch (e)
	{
	case ETimeOfDay::AFTERNOON:
		DesiredPosition = DayPositions["Afternoon"];
		break;

	case ETimeOfDay::MORNING:
		DesiredPosition = DayPositions["Morning"];
		break;

	case ETimeOfDay::EVENING:
		DesiredPosition = DayPositions["Evening"];
		break;

	case ETimeOfDay::TWILIGHT:
		DesiredPosition = DayPositions["Twilight"];
		break;
	}
	
	CurrentPosition = FMath::FInterpTo(CurrentPosition, DesiredPosition, 1.0f, TimeLength);
	//Timeline->PlayFromStart();
}

void UTimeSystem::StartDay()
{
	TimeData.Day = 1;
	TimeData.Minute = 0;
	TimeData.Hour = 14;
}

void UTimeSystem::UpdateTime()
{
	if (TimeData.Hour > 21)
	{
		TimeData.Day += 1;
		// Reset Everything
		//FOnDayChanged
	}
	
	TotalSecondsElapsed += 72; 

	int32 TotalSeconds = static_cast<int32>(TotalSecondsElapsed);

	TimeData.Hour = TotalSeconds / 3600;
	TimeData.Minute = (TotalSeconds / 60) % 60;
	UE_LOG(LogTemp, Warning, TEXT("Current Time: %d:%d"), TimeData.Hour, TimeData.Minute)

	if (TimeData.Minute >= 60)
	{
		int ExtraHours = TimeData.Minute / 60;
		TimeData.Minute = TimeData.Minute % 60;
		TimeData.Hour += ExtraHours;
	}

	for (auto Ranges : TimeData.Set)
	{
		if (TimeData.Hour >= Ranges.Key.StartHourRange && TimeData.Hour < Ranges.Key.EndHourRange)
		{
			if (CurrentTimeOfDay != Ranges.Value)
			{
				CurrentTimeOfDay = Ranges.Value;
				UE_LOG(LogTemp, Warning, TEXT("Changing LightActor Position"))
				TransitionTimeOfDay(CurrentTimeOfDay);
			}
			break; 
		}
	}
}

void UTimeSystem::OnTimelineUpdate(float val)
{
	
	StartPosition = CurrentPosition;
	
	CurrentPosition = FMath::Lerp(StartPosition, DesiredPosition, val);
	
	
	if (LightActor)
	{
		FRotator Rot = LightActor->GetActorRotation();
		Rot.Pitch = CurrentPosition;
		Rot.Yaw = 0; 
		Rot.Roll = 0; 
		LightActor->SetActorRotation(Rot);
	}
}

void UTimeSystem::InitTimeline()
{
	OnTimeline.BindUFunction(this, FName("OnTimelineUpdate"));
	Timeline = NewObject<UTimelineComponent>(this, TimelineClass);

	if (!Timeline)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create a TimelineComponent"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Timeline created"));
	}
	Timeline->RegisterComponent();
	Timeline->SetTimelineLength(7.0f);
	Timeline->AddInterpFloat(TimeCurve, OnTimeline);
	
	for (auto DayPos : DayPositions)
	{
		UE_LOG(LogTemp, Warning, TEXT("Postitons: %f"), DayPos.Value); 
	}
}

const FTimeData& UTimeSystem::GetTimeData()
{
	return TimeData;
}

void UTimeSystem::Init()
{

	Super::Init();
	DayPositions.FindOrAdd("Morning", -45.f);
	DayPositions.FindOrAdd("Evening", -10.f);
	DayPositions.FindOrAdd("Afternoon", -135.f);
	DayPositions.FindOrAdd("Twilight", 0);
	
}

void UTimeSystem::OnStart()
{
	Super::OnStart();
	UE_LOG(LogTemp, Warning, TEXT("OnStart"));
	LightActor = Cast<ADirectionalLight>(
	UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass())
);
	
	TotalSecondsElapsed = 9 * 3600.0f;
	
	TimeData.Set.Add(TTuple<FTimeRange, ETimeOfDay>(
	FTimeRange(9, 12),
	ETimeOfDay::MORNING));
	
	TimeData.Set.Add(TTuple<FTimeRange, ETimeOfDay>(
	FTimeRange(12, 15),
	ETimeOfDay::AFTERNOON));
	
	TimeData.Set.Add(TTuple<FTimeRange, ETimeOfDay>(
	FTimeRange(15, 18),
	ETimeOfDay::EVENING));
	
	TimeData.Set.Add(TTuple<FTimeRange, ETimeOfDay>(
	FTimeRange(18, 21),
	ETimeOfDay::TWILIGHT));

	StartDay();
	InitTimeline();
	
	CurrentPosition = DayPositions["Morning"];
	CurrentTimeOfDay = ETimeOfDay::MORNING;
	TransitionTimeOfDay(ETimeOfDay::MORNING);

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UTimeSystem::UpdateTime, 1.f, true);
	
	if (LightActor)
	{
		FRotator Rot;
		Rot.Roll = 0; 
		Rot.Yaw = 0;
		Rot.Pitch = -45; 
		LightActor->SetActorRotation(Rot); 
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create a TimelineComponent"));
	}
}



