// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CuisineAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KitchenOrderSubsystem.generated.h"

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
	
private:
	TArray<FKitchenOrder> KitchenOrderQueue;

	FTimerHandle OrderTimerHandle;
	void UpdateKitchenOrders();
};
