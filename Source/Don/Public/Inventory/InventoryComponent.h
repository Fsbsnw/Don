// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemAsset.h"
#include "Data/ItemStructs.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, FItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemSold, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChanged, const TArray<FItem>&, Inventory);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItem>& GetInventory() { return Inventory; }

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	TMap<FGameplayTag, int32>& GetAssignedQuickSlots() { return AssignedQuickSlots; }

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void AssignQuickSlotItem(const FGameplayTag& InputTag, const int32 InventoryIndex);
	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void UnassignQuickSlotItem(const FGameplayTag& InputTag);
	bool FindQuickSlotItemByInputTag(const FGameplayTag& InputTag) const { return AssignedQuickSlots.Contains(InputTag); };

	int32 FindItemInInventory(const FItem& Item) const;
	bool HasEnoughItems(TArray<FItem> Items);
	void SwapInventoryItems(int32 IndexA, int32 IndexB);

	const int32 MaxItemSlots = 400;

	void InitAndLoadInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BroadcastInventory();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FItem Item, int32 Amount = 1);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(int32 SlotIndex, int32 Amount = 1);
	UFUNCTION()
	void OnRequestSellItem(int32 SlotIndex);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryItemAdded;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryItemRemoved;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventorySlotChanged OnInventorySlotChanged;
	FOnInventoryItemSold OnInventoryItemSold;

	// Use

	void UseItem(int32 SlotIndex);
	void UseQuickSlotItem(const FGameplayTag& InputTag);
	void UpgradeArmorItem(int32 SlotIndex, int32 Amount = 1);

	// Store

	bool CanAffordItem(int32 Cost);
	
private:
	UPROPERTY()
	TArray<FItem> Inventory;

	UPROPERTY()
	TMap<FGameplayTag, int32> AssignedQuickSlots;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* DrinkPotion;
};
