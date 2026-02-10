// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemStructs.h"
#include "Input/DragAndDrop.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "InventoryWidgetController.generated.h"

class UInventoryComponent;
class UDonInventorySlotWidget;
class UDragDropOperation;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryWidgetChanged, const TArray<FItem>&, Inventory);
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
	virtual void BroadcastInitialValues() override;

	UInventoryComponent* GetInventoryComponent();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SlotWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HandleSlotSellEvent(int32 SlotIndex = -1);

	
	// Item functions

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItem GetItemInfo(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SwapSlotInfo(int32 FromIndex, int32 ToIndex);

	UFUNCTION(BlueprintCallable)
	void AddItemToPlayer(FItem Item, int32 Amount = 1);
	
	UFUNCTION(BlueprintCallable)
	void UseItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	bool UpgradeArmorItem(int32 SlotIndex, int32 Points = 1);
	
	UPROPERTY(BlueprintAssignable)
	FOnInventoryWidgetChanged OnInventoryChanged;
	
	UFUNCTION(BlueprintCallable)
	void HandleInventoryUpdated(const TArray<FItem>& Inventory);
};