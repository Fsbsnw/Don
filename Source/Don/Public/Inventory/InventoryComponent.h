// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemAsset.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, FItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemSold, int32, SlotIndex);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItem>& GetInventory() { return Inventory; }

	int32 FindItemInInventory(const FItem& Item) const;
	void SwapInventoryItems(int32 IndexA, int32 IndexB);

	const uint8 MaxItemSlots = 20;

	void InitAndLoadInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FItem Item, int32 Amount = 1);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FItem Item, int32 Amount = 1);
	UFUNCTION()
	void OnRequestSellItem(int32 SlotIndex);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryItemAdded;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryItemRemoved;
	FOnInventoryItemSold OnInventoryItemSold;

private:
	UPROPERTY()
	TArray<FItem> Inventory;
};
