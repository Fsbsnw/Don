// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/RoomServiceAsset.h"
#include "Inn/Character/InnCustomer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InnManagerSubsystem.generated.h"


struct FCustomerSnapshot;
class UInnCustomerGroup;
struct FSeatGroup;


USTRUCT(BlueprintType)
struct FRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsOccupied = false;

	UPROPERTY(BlueprintReadOnly)
	int32 GroupID = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Grade = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Size = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 BasePrice = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ExtraPrice = 0;
};

USTRUCT(BlueprintType)
struct FCustomer
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 GroupID = 0;

	UPROPERTY()
	TWeakObjectPtr<AInnCustomer> Customer;
	
	UPROPERTY(BlueprintReadOnly)
	ECustomerType Type = ECustomerType::First;
};

USTRUCT(BlueprintType)
struct FInnCustomerGroupSnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 GroupID = -1;
	
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* GroupPortrait = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	int32 GroupSize = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 SatisfactionScore = 0;

	UPROPERTY(BlueprintReadOnly)
	FRoomServiceOrder RoomService;
};

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnInnGroupChanged,
	const TArray<UInnCustomerGroup*>
);

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnInnGroupRequested,
	int32
);

/**
 * 
 */
UCLASS()
class DON_API UInnManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable)
	void InitInformation();

	UFUNCTION(BlueprintCallable)
	void ResetData();
	
private:	
	UPROPERTY()
	TMap<int32, UInnCustomerGroup*> InnGroups;
	
	UPROPERTY()
	TSet<int32> LodgerGroups;

	UPROPERTY()
	TArray<FSeatGroup> SeatGroups;
	
	UPROPERTY()
	TArray<FString> InnGroupFirstNames;
	UPROPERTY()
	TArray<FString> InnGroupSecondNames;
	UPROPERTY()
	TArray<FString> InnGroupThirdNames;


	int32 TodayCustomerGroupCount = 0;
	int32 NextGroupID = 1;
	int32 MaxSeatGroupSize = 12;

	bool bInitialized = false;

	void StartInnLevel();
	void ScheduleNextSpawn();
	FTimerHandle SpawnTimerHandle;
	
public:
	FString NewCustomerName();
	UInnCustomerGroup* GetGroupInfo(int32 GroupID) const;

	
	// Seat

	bool FindAndOccupyEmptySeat(int32 GroupID);

	UFUNCTION(BlueprintCallable)
	void SpawnAndRegisterCustomerGroup(FVector SpawnLocation, int32 Size = 1);

	UFUNCTION()
	void UpdateInnGroup(int32 GroupID);

	UFUNCTION()
	void OnRoomServiceReceived(int32 GroupID);
	
	void SetGroupTable(int32 GroupID, int32 TableNumber);

	ECustomerInnState GetGroupInnState(int32 GroupID);
	int32 GetGroupSize(int32 GroupID);
	// int32 GetGroupTable(int32 GroupID);

	void ReturnGroupSeats(int32 GroupID);
	void RemoveGroup(int32 GroupID);
	
	TArray<UInnCustomerGroup*> GetInnGroupsArray() const;
	TArray<UInnCustomerGroup*> GetLodgersArray() const;

	FOnInnGroupChanged OnInnGroupChanged;
	FOnInnGroupRequested OnInnGroupRoomServiceReceived;

	FVector ExitLocation = FVector::ZeroVector;
	FVector InnEntranceLocation = FVector::ZeroVector;
	FVector RoomEntranceLocation = FVector::ZeroVector;
	FVector CustomerSpawnLocation = FVector::ZeroVector;

	
	// Room

	FRoomInfo GetRoomInfo(int32 RoomNumber) { return RoomInfos[RoomNumber]; }
	FInnCustomerGroupSnapshot GetLodgerInfo(int32 RoomNumber);
	bool CheckInCustomer(int32 GroupID);

	UFUNCTION(BlueprintCallable)
	void MorningCheckOut();
	void CloseInnAtMidnight();
	
	bool AssignCustomerToRoom(int32 GroupID, int32 RoomIndex);
	bool HasEmptyRoom() const;

	UPROPERTY(BlueprintReadOnly)
	TArray<FRoomInfo> RoomInfos;

	int32 RoomCapacity = 10;
	int32 RoomServiceRevenue = 0;
};