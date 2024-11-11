// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemAsset.h"
#include "Input/DragAndDrop.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "InventoryWidgetController.generated.h"

class UDonInventorySlotWidget;
class UDragDropOperation;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdate, FItem, Item);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DON_API UInventoryWidgetController : public UDonWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdate OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdate OnInventoryItemRemoved;

	UFUNCTION()
	void OnItemAdded(FItem Item);

	UFUNCTION()
	void OnItemRemoved(FItem Item);

	UFUNCTION(BlueprintCallable)
	FItem GetItemInfo(uint8 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void CachingDraggedSlot(UDonInventorySlotWidget* DraggedSlotInfo) { DraggedSlot = DraggedSlotInfo; }

	UFUNCTION(BlueprintCallable)
	void SwapSlotInfo(UDonInventorySlotWidget* DesiredSlot);

	UFUNCTION(BlueprintCallable)
	void AddItem(FName ItemName);
	
	UFUNCTION(BlueprintCallable)
	void RemoveItem(int32 SlotIndex);
	
private:
	UDonInventorySlotWidget* DraggedSlot = nullptr;
};
