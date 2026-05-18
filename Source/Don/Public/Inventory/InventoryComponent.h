// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemStructs.h"
#include "Player/Interface/SaveableInterface.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedSignature, const TArray<FItem>&, Inventory);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInventoryComponent : public UActorComponent, public ISaveableInterface
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

// ======================================================================
// ISaveable 인터페이스
// ======================================================================
	
	virtual void SavePlayerData(FPlayerSaveData& Data) override;
	virtual void LoadPlayerData(const FPlayerSaveData& InData) override;

	
// ======================================================================
// 인벤토리 초기화
// ======================================================================

	/* 인벤토리 슬롯 사이즈 초기화 함수 */
	void InitInventory();

	
// =========================================================
// 인벤토리 조회
// =========================================================
	/* Getter */
	TArray<FItem>& GetInventory() { return Inventory; }

	/* 특정 아이템 위치 찾기 */
	int32 FindItemInInventory(const FItem& Item) const;

	/* 필요한 아이템이 충분한지 체크 */
	bool HasEnoughItems(TArray<FItem> Items) const;


// =========================================================
// 인벤토리 변경
// =========================================================
	/* 아이템 슬롯 스왑 */
	void SwapInventoryItems(int32 FromIndex, int32 ToIndex);

	/* 아이템 추가 */
	void AddItem(FItem Item, int32 Amount = 1);

	/* 아이템 제거 */
	void RemoveItemByIndex(int32 SlotIndex, int32 Amount = 1);
	void RemoveItem(const FItem& Item, int32 Amount);

	/* 아이템 사용 */
	void UseItem(int32 SlotIndex);

	
// =========================================================
// 인벤토리 Delegate
// =========================================================
	FOnInventoryChangedSignature OnInventoryChanged;

private:
// =========================================================
// Inventory Data
// =========================================================
	/* 실제 인벤토리 데이터 */
	UPROPERTY()
	TArray<FItem> Inventory;

	/* 인벤토리 슬롯 크기 */
	const int32 MaxItemSlots = 20;
};
