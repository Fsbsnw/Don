// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CuisineAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KitchenOrderSubsystem.generated.h"

class ADonPlayerState;
class AInnSeat;
class AInnCustomer;
class AInnChef;

USTRUCT(BlueprintType)
struct FCompletedFoodOrder
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ChefLevel = 1;

	UPROPERTY()
	FName FoodName;
	
	UPROPERTY()
	int32 FoodPrice = 0;

	UPROPERTY()
	FGuid OrderID = FGuid();

	UPROPERTY()
	FGuid CustomerID = FGuid();
};

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
	void BroadcastInitialValues();

	UPROPERTY()
	ADonPlayerState* DonPlayerState;
	
public:
	// Food Order	
	
	UPROPERTY(BlueprintAssignable)
	FOnKitchenOrderChanged OnKitchenOrderAdded;

	UPROPERTY(BlueprintAssignable)
	FOnKitchenOrderChanged OnKitchenOrderRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnKitchenOrderUpdated OnKitchenOrderUpdated;


	UFUNCTION(BlueprintCallable)
	TArray<FKitchenOrder>& GetKitchenOrderQueue() { return KitchenOrderQueue; };

	FKitchenOrder EnqueueKitchenOrder(FKitchenOrder& Order);
	FGuid FindNextQueuedOrderID() const;
	void UpdateKitchenOrders();

	FCompletedFoodOrder GetCompletedOrder(FGuid ID);

	
	// Chef

	void AssignChef();
	void RegisterChef(AInnChef* Chef) { if (!Chefs.Contains(Chef)) Chefs.Add(Chef); };
	void UnregisterChef(AInnChef* Chef) { if (Chefs.Contains(Chef)) Chefs.Remove(Chef); };
	AInnChef* FindIdleChef();
	
private:
	float TickTimer = 0.1f;
	
	UPROPERTY()
	TArray<FKitchenOrder> KitchenOrderQueue;

	UPROPERTY()
	TArray<AInnChef*> Chefs;

	UPROPERTY()
	TArray<FCompletedFoodOrder> CompletedFoodOrders;
	
	FTimerHandle OrderTimerHandle;
};
