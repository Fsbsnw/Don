// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemStructs.h"
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
	virtual void SetWidgetController(UObject* InWidgetController) override;

	UPROPERTY(BlueprintReadWrite)
	int32 InventorySlotIndex = -1;
	
	UFUNCTION()
	void HandleInventoryUpdated(const TArray<FItem>& Inventory);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory Slot")
	void NotifyUpdateSlot(const FItem& Item);
};