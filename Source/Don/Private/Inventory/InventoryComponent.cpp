// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "DonGameplayTags.h"
#include "Actor/DonEquipmentActor.h"
#include "Character/Interface/CombatInterface.h"
#include "Character/Player/DonCharacter.h"
#include "Data/ItemAsset.h"
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
		
		// Item == EItemType::Consumable
		if (Item.ItemType == EItemType::Consumable)
		{
			int32 Index = Inventory.Find(Item);

			// Found Item
			if (Index != INDEX_NONE)
			{
				Inventory[Index].Amount += Amount;
				OnInventoryItemAdded.Broadcast(Inventory[Index]);
				OnInventorySlotChanged.Broadcast(Inventory);
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
						OnInventorySlotChanged.Broadcast(Inventory);
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
					OnInventorySlotChanged.Broadcast(Inventory);
				}
			}
			DonPlayerState->CheckQuestObjectives();
		}
	}
}

void UInventoryComponent::RemoveItem(FItem Item, int32 SlotIndex, int32 Amount)
{
	if (SlotIndex != INDEX_NONE)
	{
		// if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetOwner()))
		// {
		// 	ADonCharacter* ControlledCharacter = Cast<ADonCharacter>(DonPlayerState->GetPawn());
		// 	if (ControlledCharacter && ControlledCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsItemEquipped(ControlledCharacter, Inventory[SlotIndex]))
		// 	{
		// 		ICombatInterface::Execute_UnequipItem(ControlledCharacter, Inventory[SlotIndex]);
		// 	}
		// }
		
		if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetOwner()))
		{
			if (!DonPlayerState)
			{
				UE_LOG(LogTemp, Error, TEXT("DonPlayerState is nullptr!"));
				return;
			}

			APawn* Pawn = DonPlayerState->GetPawn();
			if (!Pawn)
			{
				UE_LOG(LogTemp, Error, TEXT("GetPawn() returned nullptr!"));
				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("Pawn Class: %s"), *Pawn->GetClass()->GetName());

			ADonCharacter* ControlledCharacter = Cast<ADonCharacter>(Pawn);
			if (!ControlledCharacter)
			{
				UE_LOG(LogTemp, Error, TEXT("Pawn is not ADonCharacter!"));
				return;
			}

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
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GetOwner() is not an ADonPlayerState!"));
		}

		
		if (Inventory[SlotIndex].Amount - Amount > 0)
		{
			Inventory[SlotIndex].Amount -= Amount;
		}
		else
		{
			FItem DefaultItem;
			DefaultItem.Amount = 0;
			DefaultItem.InventorySlotIndex = SlotIndex;
			Inventory[SlotIndex] = DefaultItem;
		}
		OnInventoryItemRemoved.Broadcast(Inventory[SlotIndex]);
		OnInventorySlotChanged.Broadcast(Inventory);
	}
}

void UInventoryComponent::OnRequestSellItem(int32 SlotIndex)
{
	OnInventoryItemSold.Broadcast(SlotIndex);
}

void UInventoryComponent::EquipArmorItem(int32 SlotIndex)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetOwner()))
	{
		ADonCharacter* ControlledCharacter = Cast<ADonCharacter>(DonPlayerState->GetPawn());
		
		if (ControlledCharacter && ControlledCharacter->Implements<UCombatInterface>())
		{
			const FDonGameplayTags& GameplayTags = FDonGameplayTags::Get();
			FItem ItemToEquip = Inventory[SlotIndex];
			
			if (ItemToEquip.ItemTag.MatchesTagExact(GameplayTags.Item_Equippable_Armor_Helmet))
			{
				ICombatInterface::Execute_EquipArmorHelmet(ControlledCharacter, ItemToEquip);
			}
			else if (ItemToEquip.ItemTag == GameplayTags.Item_Equippable_Armor_Chest)
			{
				ICombatInterface::Execute_EquipArmorChest(ControlledCharacter, ItemToEquip);
			}
			else if (ItemToEquip.ItemTag == GameplayTags.Item_Equippable_Armor_Hands)
			{
				ICombatInterface::Execute_EquipArmorHands(ControlledCharacter, ItemToEquip);
			}
			else if (ItemToEquip.ItemTag == GameplayTags.Item_Equippable_Armor_Legs)
			{
				ICombatInterface::Execute_EquipArmorLegs(ControlledCharacter, ItemToEquip);
			}
			else if (ItemToEquip.ItemTag == GameplayTags.Item_Equippable_Armor_Boots)
			{
				ICombatInterface::Execute_EquipArmorBoots(ControlledCharacter, ItemToEquip);
			}
			else if (ItemToEquip.ItemTag == GameplayTags.Item_Equippable_Weapon)
			{
				ICombatInterface::Execute_EquipWeapon(ControlledCharacter, ItemToEquip);
			}
		}
	}
}

void UInventoryComponent::UseConsumableItem(int32 SlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Use Consumable Item : %s"), *Inventory[SlotIndex].ItemName.ToString());
}

void UInventoryComponent::UseQuickSlotItem(const FGameplayTag& InputTag)
{
	int32 FoundInventoryIndex = *AssignedQuickSlots.Find(InputTag);
	if (Inventory[FoundInventoryIndex].ItemType == EItemType::Consumable)
	{
		UseConsumableItem(FoundInventoryIndex);
	}
	else if (Inventory[FoundInventoryIndex].ItemType == EItemType::Equipable)
	{
		EquipArmorItem(FoundInventoryIndex);
	}
}
