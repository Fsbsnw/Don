// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemAsset.h"
#include "Input/DragAndDrop.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "InventoryWidgetController.generated.h"

class UInventoryComponent;
class UDonInventorySlotWidget;
class UDragDropOperation;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdate, FItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, Money);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DON_API UInventoryWidgetController : public UDonWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;

	UFUNCTION(BlueprintCallable)
	void InitAndLoadInventory();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SlotWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HandleSlotSellEvent(int32 SlotIndex = -1);

	UInventoryComponent* GetInventoryComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdate OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdate OnInventoryItemRemoved;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnMoneyChanged OnMoneyChanged;

	// Item functions
	
	UFUNCTION()
	void OnItemAdded(FItem Item);

	UFUNCTION()
	void OnItemRemoved(FItem Item);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItem GetItemInfo(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void CachingDraggedSlot(UDonInventorySlotWidget* DraggedSlotInfo) { DraggedSlot = DraggedSlotInfo; }

	UFUNCTION(BlueprintCallable)
	void SwapSlotInfo(UDonInventorySlotWidget* DesiredSlot);

	UFUNCTION(BlueprintCallable)
	void AddItemToPlayer(FItem Item, int32 Amount = 1);
	
	UFUNCTION(BlueprintCallable)
	void RemoveItemFromPlayer(FItem Item, int32 Amount = 1);

	// Money functions
	
	UFUNCTION()
	void OnMoneyAdded(int32 Money);
	
private:
	UDonInventorySlotWidget* DraggedSlot = nullptr;
};
