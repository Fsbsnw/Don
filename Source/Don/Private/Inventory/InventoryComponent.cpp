// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Data/ItemAsset.h"
#include "Player/DonPlayerState.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

int32 UInventoryComponent::FindItemInInventory(const FItem& Item) const
{
	return Inventory.Find(Item);
}

void UInventoryComponent::SwapInventoryItems(int32 IndexA, int32 IndexB)
{
	FItem CachedItem = Inventory[IndexA];

	Inventory[IndexA] = Inventory[IndexB];
	Inventory[IndexB] = CachedItem;

	Inventory[IndexA].InventorySlotIndex = IndexA;
	Inventory[IndexB].InventorySlotIndex = IndexB;
}

void UInventoryComponent::InitAndLoadInventory()
{
	Inventory.SetNum(MaxItemSlots);
	for (uint8 i = 0; i < MaxItemSlots; i++)
	{
		Inventory[i].InventorySlotIndex = i;
	}

	// Load Inventory Info
}

void UInventoryComponent::AddItem(FItem Item, int32 Amount)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetOwner()))
	{		
		if (Item.ItemName.IsNone()) return;

		// Item == EItemType::Consumable
		if (Item.ItemType == EItemType::Consumable)
		{
			int32 Index = Inventory.Find(Item);

			// Found Item
			if (Index != INDEX_NONE)
			{
				Inventory[Index].Amount += Amount;
				OnInventoryItemAdded.Broadcast(Inventory[Index]);
				DonPlayerState->CheckQuestObjectives();
			}
			// Cannot Found Item
			else
			{
				for (uint8 i = 0; i < MaxItemSlots; i++)
				{
					if (Inventory[i].ItemName.IsNone())
					{
						Item.Amount = Amount;
						Item.InventorySlotIndex = i;
						Inventory[i] = Item;
						OnInventoryItemAdded.Broadcast(Inventory[i]);
						DonPlayerState->CheckQuestObjectives();
						return;
					}
				}
			}
		}
		// ItemType != EItemType::Consumable
		else
		{
			for (uint8 i = 0; i < MaxItemSlots; i++)
			{
				if (Inventory[i].ItemName.IsNone())
				{
					if (Amount <= 0) break;

					Item.InventorySlotIndex = i;
					Inventory[i] = Item;
					Amount--;
					OnInventoryItemAdded.Broadcast(Inventory[i]);
				}
			}
			DonPlayerState->CheckQuestObjectives();
		}
	}
}

void UInventoryComponent::RemoveItem(FItem Item, int32 Amount)
{
	int32 Index = Inventory.Find(Item);

	if (Index != INDEX_NONE)
	{
		if (Inventory[Index].Amount - Amount > 0)
		{
			Inventory[Index].Amount -= Amount;
		}
		else
		{
			FItem DefaultItem;
			DefaultItem.Amount = 0;
			DefaultItem.InventorySlotIndex = Index;
			Inventory[Index] = DefaultItem;
		}
		OnInventoryItemRemoved.Broadcast(Inventory[Index]);
	}
}

void UInventoryComponent::OnRequestSellItem(int32 SlotIndex)
{
	OnInventoryItemSold.Broadcast(SlotIndex);
}
