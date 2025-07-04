// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemAsset.h"
#include "UI/Widget/DonUserWidget.h"
#include "MerchandiseSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UMerchandiseSlotWidget : public UDonUserWidget
{
	GENERATED_BODY()
public:
	virtual void SetWidgetController(UObject* InWidgetController) override;

	UPROPERTY(BlueprintReadWrite)
	int32 InventorySlotIndex = -1;

	UFUNCTION(BlueprintCallable, Category = "Merchandise Slot")
	void UpdateSlotInfo(FItem ItemInfo);

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot")
	void NotifyUpdateSlot(const FItem& Item);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	void RemoveMerchandise(FItem Item, int32 AmountToRemove = 1);
};
