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
	
public:
	// Kitchen Order
	
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
	
	void RegisterChef(AInnChef* Chef) { if (!Chefs.Contains(Chef)) Chefs.Add(Chef); };
	void UnregisterChef(AInnChef* Chef) { if (Chefs.Contains(Chef)) Chefs.Remove(Chef); };
	AInnChef* FindIdleChef();

	
	// Seat

	AInnSeat* FindEmptySeat(AInnCustomer* Customer);
	void AssignSeatToCustomer(AInnCustomer* Customer);


	// Test

	UFUNCTION(BlueprintCallable)
	void SpawnKitchenCustomer(TSubclassOf<AInnCustomer> CustomerClass, FVector SpawnLocation);
	
private:
	TArray<FKitchenOrder> KitchenOrderQueue;
	TArray<AInnChef*> Chefs;
	TArray<AInnCustomer*> Customers;
	TArray<AInnSeat*> Seats;

	FTimerHandle OrderTimerHandle;
	void UpdateKitchenOrders();
};
