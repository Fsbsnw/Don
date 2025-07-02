// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DonGameplayTags.h"
#include "Character/Interface/CombatInterface.h"
#include "Character/Player/DonCharacter.h"
#include "Data/DonItemBase.h"
#include "Data/ItemAsset.h"
#include "Inventory/DonItemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AssignQuickSlotItem(const FGameplayTag& InputTag, const int32 InventoryIndex)
{
	AssignedQuickSlots.Add(InputTag, InventoryIndex);
}

void UInventoryComponent::UnassignQuickSlotItem(const FGameplayTag& InputTag)
{
	AssignedQuickSlots.Remove(InputTag);
}

int32 UInventoryComponent::FindItemInInventory(const FItem& Item) const
{
	return Inventory.Find(Item);
}

bool UInventoryComponent::HasEnoughItems(TArray<FItem> Items)
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
	for (int32 i = 0; i < MaxItemSlots; i++)
	{
		Inventory[i].InventorySlotIndex = i;
	}

	// Load Inventory Info
}

void UInventoryComponent::BroadcastInventory()
{
	OnInventorySlotChanged.Broadcast(Inventory);
}

void UInventoryComponent::AddItem(FItem Item, int32 Amount)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetOwner()))
	{		
		if (Item.ItemName.IsNone()) return;

		Item.Amount = Amount;

		FDonGameplayTags Tags = FDonGameplayTags::Get();
		// ItemType == EItemType::Equipable
		if (Item.ItemTag.MatchesTag(Tags.Item_Equippable))
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
					OnInventorySlotChanged.Broadcast(Inventory);
				}
			}
			DonPlayerState->CheckAllQuestObjectives();
		}
		// ItemType != EItemType::Equipable
		else
		{
			int32 Index = Inventory.Find(Item);

			// Found Item
			if (Index != INDEX_NONE)
			{
				Inventory[Index].Amount += Amount;
				OnInventoryItemAdded.Broadcast(Inventory[Index]);
				OnInventorySlotChanged.Broadcast(Inventory);
				DonPlayerState->CheckAllQuestObjectives();
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
						OnInventorySlotChanged.Broadcast(Inventory);
						DonPlayerState->CheckAllQuestObjectives();
						return;
					}
				}
			}
		}
	}
}

void UInventoryComponent::RemoveItem(int32 SlotIndex, int32 Amount)
{
	if (SlotIndex != INDEX_NONE)
	{
		if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetOwner()))
		{
			APawn* Pawn = DonPlayerState->GetPawn();
			ADonCharacter* ControlledCharacter = Cast<ADonCharacter>(Pawn);

			if (!ControlledCharacter->Implements<UCombatInterface>())
			{
				UE_LOG(LogTemp, Error, TEXT("ControlledCharacter does not implement UCombatInterface!"));
				return;
			}

			if (ICombatInterface::Execute_IsItemEquipped(ControlledCharacter, Inventory[SlotIndex]))
			{
				ICombatInterface::Execute_UnequipItem(ControlledCharacter, Inventory[SlotIndex]);
			}
		}

		FItem Item = Inventory[SlotIndex];
		int32 RemainingAmount = Amount;
		int32 IndexToCheck = SlotIndex;

		while (RemainingAmount > 0 && IndexToCheck != INDEX_NONE)
		{
			if (Inventory[IndexToCheck].Amount - RemainingAmount > 0)
			{
				Inventory[IndexToCheck].Amount -= RemainingAmount;
				RemainingAmount = 0;				
			}
			else
			{
				RemainingAmount -= Inventory[IndexToCheck].Amount;
				FItem DefaultItem;
				DefaultItem.Amount = 0;
				DefaultItem.InventorySlotIndex = IndexToCheck;
				Inventory[IndexToCheck] = DefaultItem;
			}
			OnInventoryItemRemoved.Broadcast(Inventory[IndexToCheck]);
			OnInventorySlotChanged.Broadcast(Inventory);

			IndexToCheck = FindItemInInventory(Item);
		}
	}
}

void UInventoryComponent::OnRequestSellItem(int32 SlotIndex)
{
	OnInventoryItemSold.Broadcast(SlotIndex);
}

void UInventoryComponent::UseItem(int32 SlotIndex)
{
	UDonItemBase* Item = UDonItemLibrary::CreateItemObjectByTag(this, Inventory[SlotIndex].ItemTag);
	bool bWasConsumed = false;
	Item->UseItem(GetOwner(), Inventory[SlotIndex], bWasConsumed);
	if (bWasConsumed) RemoveItem(SlotIndex, 1);
}

void UInventoryComponent::UseQuickSlotItem(const FGameplayTag& InputTag)
{
	int32 FoundInventoryIndex = *AssignedQuickSlots.Find(InputTag);
	UseItem(FoundInventoryIndex);
}

void UInventoryComponent::UpgradeArmorItem(int32 SlotIndex, int32 Amount)
{
	Inventory[SlotIndex].EquipmentAttribute.Upgrade += Amount;

	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetOwner()))
	{
		ADonCharacter* ControlledCharacter = Cast<ADonCharacter>(DonPlayerState->GetPawn());
		
		if (ControlledCharacter && ControlledCharacter->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_UpdateUpgradedItemInfo(ControlledCharacter, Inventory[SlotIndex]);
		}
	}
}

bool UInventoryComponent::CanAffordItem(int32 Cost)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetOwner()))
	{
		return DonPlayerState->GetMoney() >= Cost;
	}
	return false;
}
