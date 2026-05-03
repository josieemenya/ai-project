// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AITypes.h"
#include "Components/ActorComponent.h"
#include "ExitSequence.h"
#include "TimeSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Routine.generated.h"



class UTimeSystem;
class ASmartObject; 

struct FTimeRange; 

UENUM(Blueprintable)
enum class ETimeRoutine : uint8
{
	ROLLCALL, // 9 - 9:10
	BREAKFAST, // 9 : 15 - 9 : 45
	FREETIME, // stop until 10 : 30
	LUNCH,
	WORK, // until 2 : 30
	EXERCISE, // 3:30 - 4:30
	SHOWER, //until 5:15
	DINNER, // 6:30 - 7:00
	LIGHTSOUT // 21
};

USTRUCT(BlueprintType)
struct FRoutineState
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETimeRoutine TimeRoutine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Animation;
	
	bool operator==(const FRoutineState& other) const
	{
		return TimeRoutine == other.TimeRoutine;
	} 
};

USTRUCT(BlueprintType)
struct FRoutineElem : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimeRange Range; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRoutineState State;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API URoutineComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	
	URoutineComponent();
	
	
	DECLARE_DELEGATE(FOnCurrentRoutineChanged); 
	
	FOnCurrentRoutineChanged OnCurrentRoutineChanged;
	
	void Initialize();
	
	UTimeSystem* TimeSystem;
	
	FRoutineState GetCurrentState(); 
	
	FRoutineState LastCurrentState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* RoutineTable; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRoutineState CurrentState;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoutineElem*> AllRoutines;
	
	void MoveSuccessful(FAIRequestID RequestID, EPathFollowingResult::Type Result); 
	
	UFUNCTION(BlueprintCallable)
	EExitSequenceType TransitionRoutine(); 
	
	UFUNCTION(BlueprintCallable)
	EExitSequenceType RoutineSequence(); // PlayAnimation
	
	bool bHasStateChanged() const;
	bool WithinTimeRange(const FTimeData& Data, const FTimeRange& TimeRange) const; 
	//bool WithinMinuteRange(const FTimeData& Data, const FTimeRange& TimeRange) const;
	
protected:
	virtual void BeginPlay() override;
};


class UGuardRoutineComponent : public URoutineComponent
{
public:
	
	UGuardRoutineComponent();
};
