// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CuisineAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KitchenOrderSubsystem.generated.h"

class AInnSeat;
class AInnCustomer;
class AInnChef;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKitchenOrderChanged, FKitchenOrder, KitchenOrder);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKitchenOrderUpdated, const TArray<FKitchenOrder>&, KitchenOrders);

/**
 * 
 */
UCLASS()
class DON_API UKitchenOrderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	UFUNCTION(BlueprintCallable)
	void InitDestLocations();
	
public:
	// Kitchen Order

	float TickTimer = 0.1f;
	
	UPROPERTY(BlueprintAssignable)
	FOnKitchenOrderChanged OnKitchenOrderAdded;

	UPROPERTY(BlueprintAssignable)
	FOnKitchenOrderChanged OnKitchenOrderRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnKitchenOrderUpdated OnKitchenOrderUpdated;

	UFUNCTION(BlueprintCallable)
	void BroadcastInitialValues();

	UFUNCTION(BlueprintCallable)
	TArray<FKitchenOrder>& GetKitchenOrderQueue() { return KitchenOrderQueue; };

	FKitchenOrder EnqueueKitchenOrder(FKitchenOrder& Order);
	FGuid FindNextQueuedOrderID() const;

	
	// Chef

	void AssignChef(FKitchenOrder& Order);
	void RegisterChef(AInnChef* Chef) { if (!Chefs.Contains(Chef)) Chefs.Add(Chef); };
	void UnregisterChef(AInnChef* Chef) { if (Chefs.Contains(Chef)) Chefs.Remove(Chef); };
	AInnChef* FindIdleChef();

	
	// Seat

	AInnSeat* FindAndOccupyEmptySeat();
	bool HasEmptySeat();


	// Test

	UFUNCTION(BlueprintCallable)
	void SpawnKitchenCustomer(TSubclassOf<AInnCustomer> CustomerClass, FVector SpawnLocation);

	UPROPERTY(BlueprintReadWrite)
	FVector ExitLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	FVector InnEntranceLocation = FVector::ZeroVector;;
	UPROPERTY(BlueprintReadWrite)
	FVector RoomEntranceLocation = FVector::ZeroVector;;
	
private:
	UPROPERTY()
	TArray<FKitchenOrder> KitchenOrderQueue;

	UPROPERTY()
	TArray<AInnChef*> Chefs;

	UPROPERTY()
	TArray<AInnCustomer*> Customers;

	UPROPERTY()
	TArray<AInnSeat*> Seats;

	FTimerHandle OrderTimerHandle;
	void UpdateKitchenOrders();
};
