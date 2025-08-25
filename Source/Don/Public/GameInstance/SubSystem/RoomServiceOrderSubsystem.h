// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/RoomServiceAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RoomServiceOrderSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomServiceOrderChanged, FRoomServiceOrder, RoomServiceOrder);

/**
 * 
 */
UCLASS()
class DON_API URoomServiceOrderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
public:
	UFUNCTION(BlueprintCallable)
	void BroadcastInitialValues();
	
	UPROPERTY(BlueprintAssignable)
	FOnRoomServiceOrderChanged OnRoomServiceOrderChanged;
	
	UFUNCTION(BlueprintCallable)
	TArray<FRoomServiceOrder>& GetRoomServiceOrderQueue() { return RoomServiceOrderQueue; };

	FRoomServiceOrder EnqueueRoomServiceOrder(const FRoomServiceOrder& Order);
	FRoomServiceOrder DequeueRoomServiceOrder();

	
private:
	TArray<FRoomServiceOrder> RoomServiceOrderQueue;
};
