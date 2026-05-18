// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "DonGameplayTags.h"
#include "Character/Interface/CombatInterface.h"
#include "Data/DonItemBase.h"
#include "Inventory/DonItemLibrary.h"
#include "Player/DonPlayerState.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::SavePlayerData(FPlayerSaveData& Data)
{
	Data.Inventory = Inventory;
}

void UInventoryComponent::LoadPlayerData(const FPlayerSaveData& InData)
{
	Inventory = InData.Inventory;

	if (Inventory.IsEmpty())
	{
		InitInventory();
	}
}

void UInventoryComponent::InitInventory()
{
	Inventory.SetNum(MaxItemSlots);
}

int32 UInventoryComponent::FindItemInInventory(const FItem& Item) const
{
	if (Item.ItemName.IsNone()) return -1;
	return Inventory.Find(Item);
}

bool UInventoryComponent::HasEnoughItems(TArray<FItem> Items) const
{
	for (FItem Item : Items)
	{
		int32 ItemIndex = FindItemInInventory(Item);
		if (ItemIndex < 0 || ItemIndex >= 0 && Inventory[ItemIndex].Amount < Item.Amount)
		{
			return false;
		}
	}
	return true;
}

void UInventoryComponent::SwapInventoryItems(int32 FromIndex, int32 ToIndex)
{
	Inventory.Swap(FromIndex, ToIndex);	
	OnInventoryChanged.Broadcast(Inventory);
}

void UInventoryComponent::AddItem(FItem Item, int32 Amount)
{
	if (Item.ItemName.IsNone())
	{
		return;
	}

	const FDonGameplayTags& Tags = FDonGameplayTags::Get();
	
	// Stack이 1인 아이템의 경우 분산 배치
	if (Item.ItemTag.MatchesTag(Tags.Item_Equippable))
	{
		for (int32 i = 0; i < MaxItemSlots && Amount > 0; ++i)
		{
			if (Inventory[i].ItemName.IsNone())
			{
				Inventory[i] = Item;
				--Amount;
			}
		}
	}
	// Stack이 여러 개인 아이템의 경우 
	else
	{
		// 기존 아이템 존재 여부 확인
		const int32 Index = Inventory.Find(Item);

		if (Index != INDEX_NONE)
		{
			// 기존 슬롯에 수량 누적
			Inventory[Index].Amount += Amount;
		}
		else
		{
			// 빈 슬롯에 신규 삽입
			for (uint8 i = 0; i < MaxItemSlots; i++)
			{
				if (Inventory[i].ItemName.IsNone())
				{
					Item.Amount = Amount;
					Inventory[i] = Item;
					break;
				}
			}
		}
	}
	OnInventoryChanged.Broadcast(Inventory);
}

void UInventoryComponent::RemoveItemByIndex(int32 SlotIndex, int32 Amount)
{
	if (SlotIndex == INDEX_NONE || !Inventory.IsValidIndex(SlotIndex))
	{
		return;
	}

	// 아이템 제거 전에 장착 중인 아이템인 경우 해제
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetOwner()))
	{
		APawn* Pawn = DonPlayerState->GetPawn();

		if (Pawn && Pawn->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_UnequipItem(Pawn, Inventory[SlotIndex]);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Pawn does not implement UCombatInterface!"));
		}
	}

	// 아이템 제거 실행
	FItem& Item = Inventory[SlotIndex];

	if (Item.Amount > Amount)
	{
		// 일부 감소
		Item.Amount -= Amount;
	}
	else
	{
		// 슬롯 비우기
		Item = FItem();
		Item.InventorySlotIndex = SlotIndex;
	}
	
	OnInventoryChanged.Broadcast(Inventory);
}

void UInventoryComponent::RemoveItem(const FItem& Item, int32 Amount)
{
	if (Item.ItemName.IsNone() || Amount <= 0)
	{
		return;
	}

	int32 RemainingAmount = Amount;

	// 인벤토리 순회
	for (int32 i = 0; i < Inventory.Num() && RemainingAmount > 0; ++i)
	{
		FItem& InventoryItem = Inventory[i];

		// 동일 아이템이 아니면 skip
		if (InventoryItem.ItemName != Item.ItemName)
		{
			continue;
		}

		// 아이템 제거
		if (InventoryItem.Amount > RemainingAmount)
		{
			InventoryItem.Amount -= RemainingAmount;
			RemainingAmount = 0;
		}
		// 아이템 제거 및 슬롯 초기화
		else
		{
			RemainingAmount -= InventoryItem.Amount;
			InventoryItem = FItem();
			InventoryItem.InventorySlotIndex = i;
		}
	}
	OnInventoryChanged.Broadcast(Inventory);
}

void UInventoryComponent::UseItem(int32 SlotIndex)
{
	UDonItemBase* Item = UDonItemLibrary::CreateItemObjectByTag(this, Inventory[SlotIndex].ItemTag);
	bool bWasConsumed = false;
	Item->UseItem(GetOwner(), Inventory[SlotIndex], bWasConsumed);
	if (bWasConsumed) RemoveItemByIndex(SlotIndex, 1);
}
