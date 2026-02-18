// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoomComponent.generated.h"

UENUM(BlueprintType)
enum class EDirection : uint8
{
	None = 0,
	NORTH = 1,
	SOUTH = 2,
	EAST = 3,
	WEST = 4,
};

DECLARE_MULTICAST_DELEGATE(FOnRoomChange);

UCLASS()
class AI_PROJECT_API URoom : public UObject
{
	GENERATED_BODY()
	
public:
	URoom() = default;
	
	FString ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ULevel* RoomLevel; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EDirection, URoom*> DoorMap;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_PROJECT_API URoomComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoomComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping")
	URoom* CurrentRoom ; 
	
	FOnRoomChange ChangeRoom; 
	
	EDirection Direction;
	
	void SetCurrentRoom(URoom* NewRoom);
	URoom* FindRoom();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
