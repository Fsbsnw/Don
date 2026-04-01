// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemStructs.h"
#include "Player/Interface/SaveableInterface.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, FItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedSignature, const TArray<FItem>&, Inventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemSold, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChanged, const TArray<FItem>&, Inventory);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInventoryComponent : public UActorComponent, public ISaveableInterface
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void SavePlayerData(FPlayerSaveData& Data) override;
	virtual void LoadPlayerData(const FPlayerSaveData& InData) override;

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
	void SwapInventoryItems(int32 FromIndex, int32 ToIndex);

	const int32 MaxItemSlots = 20;

	void InitInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BroadcastInventory();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FItem Item, int32 Amount = 1);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(int32 SlotIndex, int32 Amount = 1);
	void SellItem(FItem Item, int32 Amount = 1);
	UFUNCTION()
	void OnRequestSellItem(int32 SlotIndex);

	FOnInventoryChangedSignature OnInventoryChanged;
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
	USoundBase* DrinkPotion = nullptr;
};
