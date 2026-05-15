// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Engine/World.h"
#include "UObject/NoExportTypes.h"
#include "TimeSystem.generated.h"

/**
 * 
 */

class UTimelineComponent;

UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
	MORNING, 
	EVENING, 
	AFTERNOON, 
	TWILIGHT
};

USTRUCT(BlueprintType)
struct FTimeRange
{
	
	GENERATED_BODY()
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StartHourRange; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EndHourRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StartMinuteRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EndMinuteRange;
	
};

USTRUCT(BlueprintType)
struct FTimeData
{
	GENERATED_BODY()
	public:
	
	TArray<TPair<FTimeRange, ETimeOfDay>> Set;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Hour; 
	
	UPROPERTY(BlueprintReadWrite)
	int32 Minute;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Day; 
	
	
	
};
 
UCLASS(Blueprintable)
class AI_PROJECT_API UTimeSystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	
	UTimeSystem(); 
	
	//UTime
	
	UPROPERTY(BlueprintReadOnly)
	ETimeOfDay CurrentTimeOfDay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimeData TimeData;
	

	UFUNCTION(BlueprintCallable)
	void TransitionTimeOfDay(ETimeOfDay e);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTimelineComponent> TimelineClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* Timeline;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* TimeCurve;
	
	FOnTimelineFloat OnTimeline;
	
	UFUNCTION(BlueprintCallable)
	void StartDay(); 
	
	UFUNCTION(BlueprintCallable)
	void UpdateTime();
	void OnTimelineUpdate(float val);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPosition; 
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, float> DayPositions;
	
	TArray<FString> Times;
	float DesiredPosition;


	void InitTimeline();
	const FTimeData& GetTimeData();
	void Init();


	class ADirectionalLight* LightActor; 
	
	float TotalSecondsElapsed;
	
	float StartPosition;
	
	float LastDesiredPosition;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimelineUpdated,  const FTimeData&, TimeData);

	FOnTimelineUpdated OnTimelineUpdated;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	
	UFUNCTION(BlueprintCallable)
	bool WithinTimeRange(const FTimeData& Data, const FTimeRange& TimeRange) const;

	bool DoesSupportWorldType(const EWorldType::Type WorldType) const override {
		return WorldType == EWorldType::PIE or WorldType == EWorldType::Game;	
	}
	
};
