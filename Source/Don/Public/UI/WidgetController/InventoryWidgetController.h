// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemAsset.h"
#include "Data/ItemStructs.h"
#include "Input/DragAndDrop.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "InventoryWidgetController.generated.h"

class UInventoryComponent;
class UDonInventorySlotWidget;
class UDragDropOperation;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdate, FItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceChanged, int32, Money);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DON_API UInventoryWidgetController : public UDonWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UInventoryComponent* GetInventoryComponent();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SlotWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HandleSlotSellEvent(int32 SlotIndex = -1);

	/*
	 * Delegates
	 */
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdate OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdate OnInventoryItemRemoved;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnResourceChanged OnMoneyChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnResourceChanged OnMemoryFragmentChanged;

	/*
	 * Delegates
	 */

	
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

	UFUNCTION(BlueprintCallable)
	void UnequipAllItems();
	
	UFUNCTION(BlueprintCallable)
	void UseItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	bool UpgradeArmorItem(int32 SlotIndex, int32 Points = 1);
	
	// Resource functions
	
	UFUNCTION()
	void OnMoneyAdded(int32 Money);
	
	UFUNCTION()
	void OnMemoryFragmentAdded(int32 MemoryFragment);

	UFUNCTION(BlueprintCallable)
	void AddMemoryFragment(int32 InMemoryFragment);
private:
	UDonInventorySlotWidget* DraggedSlot = nullptr;
};