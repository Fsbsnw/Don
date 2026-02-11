// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/RoomServiceAsset.h"
#include "Inn/Character/InnCustomer.h"
#include "InnCustomerGroup.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGroupChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoomServiceChanged, float);

struct FCustomer;
class AInnCustomer;

USTRUCT()
struct FCustomerSnapshot
{
	GENERATED_BODY()

	UPROPERTY()
	ECustomerType Type = ECustomerType::First;

	UPROPERTY()
	int32 Level = 1;
};

/**
 * 
 */
UCLASS()
class DON_API UInnCustomerGroup : public UObject
{
	GENERATED_BODY()

public:
	// Common

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Portrait;
	
	UPROPERTY(BlueprintReadOnly)
	int32 GroupID = -1;
	UPROPERTY(BlueprintReadOnly)
	FString GroupName;
	FOnGroupChanged OnGroupChanged;
	FOnGroupChanged OnGroupRoomServiceRequested;
	FOnRoomServiceChanged OnRoomServiceChanged;
	
	// Kitchen Services
	
	void RequestSeats();
	void OnMemberNotified(ECustomerNotify Notify);
	void FinishedEating();
	void DecideToLodge();

	UPROPERTY(BlueprintReadOnly)
	TArray<AInnCustomer*> MemberActors;
	UPROPERTY()
	TArray<FCustomerSnapshot> MemberSnapshots;

	UPROPERTY(BlueprintReadOnly)
	ECustomerInnState InnState = ECustomerInnState::Entrance;
	ECustomerMealState MealState = ECustomerMealState::WaitingForFood;

	UPROPERTY(BlueprintReadOnly)
	int32 AssignedTable = -1;
	UPROPERTY(BlueprintReadOnly)
	int32 AssignedRoomNumber = -1;

	UPROPERTY(BlueprintReadOnly)
	int32 Satisfaction = 0;

	void AddToSatisfaction(int32 InSatisfaction);
	
	// Room Services
	
	FCustomerSnapshot MakeCustomerSnapshot(AInnCustomer* Customer);
	void EnterRoom();
	void RespawnMembers();
	void PayCost();
	void CheckOut();
	
	FRoomServiceOrder GroupRoomService;
	void SetRoomServiceTimer();
	void InitRoomService();
	void RequestRoomService();
	void TickRoomService();
	float MaxRoomServiceTimeLimit;

	FTimerHandle RoomServiceTimeLimitHandle;

	void ExitInn();
	
private:
	int32 FinishedEatingCount = 0;
	int32 EnteredMemberCount = 0;
	int32 EnteredRoomMemberCount = 0;
	int32 ExitMemberCount = 0;
	
};
