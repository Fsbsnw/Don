// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemAsset.h"
#include "UI/Widget/DonUserWidget.h"
#include "DonInventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UDonInventorySlotWidget : public UDonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FName ItemName = FName("NONE");
	
	UPROPERTY(BlueprintReadWrite)
	FGuid ItemID;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Amount = 0;

	UPROPERTY(BlueprintReadWrite)
	float Durability = 0.f;

	UPROPERTY(BlueprintReadWrite)
	int32 Upgrade = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 InventorySlotIndex = -1;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	UFUNCTION(BlueprintCallable)
	void UpdateSlotInfo(FItem ItemInfo);

	UFUNCTION(BlueprintNativeEvent)
	void NotifyUpdateSlot();

	UFUNCTION(BlueprintCallable)
	void CachingDraggedSlotInfo(UDonInventorySlotWidget* InSlotWidget);

	UFUNCTION(BlueprintCallable)
	void SwapSlotInfo(UDonInventorySlotWidget* InSlotWidget);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(int32 SlotIndex);
};
