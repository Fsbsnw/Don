// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Data/ItemAsset.h"
#include "Inventory/DonItemLibrary.h"
#include "Player/DonPlayerState.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddItem(APlayerState* PlayerState, FName ItemName)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		uint8 MaxItemSlots = DonPlayerState->MaxItemSlots;
		
		FItem ItemToAdd = UDonItemLibrary::FindItemByName(this, ItemName);
		if (ItemToAdd.ItemName == FName("NONE")) return;

		bool bHasEmptySlot = false;
		for (uint8 i = 0; i < MaxItemSlots; i++)
		{
			if (DonPlayerState->GetInventory()[i].ItemName == FName("NONE"))
			{
				ItemToAdd.InventorySlotIndex = i;
				DonPlayerState->GetInventory()[i] = ItemToAdd;
				bHasEmptySlot = true;
				break;
			}
		}

		if (!bHasEmptySlot) return; 

		OnInventoryItemAdded.Broadcast(ItemToAdd);
	}
}

void UInventoryComponent::RemoveItem(APlayerState* PlayerState, FName ItemName)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		// FItem ItemToRemove;
		// for (FItem Item : DonPlayerState->Inventory)
		// {
		// 	if (Item.ItemName == ItemName)
		// 	{
		// 		ItemToRemove = Item;
		// 	}
		// }

		
		FItem ItemToAdd = UDonItemLibrary::FindItemByName(this, ItemName);
		if (ItemToAdd.ItemName == FName("NONE")) return;
		
		for (const FItem Item : DonPlayerState->GetInventory())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Item is Removed"), *Item.ItemName.ToString());
		}

		OnInventoryItemRemoved.Broadcast(ItemToAdd);
	}
}
