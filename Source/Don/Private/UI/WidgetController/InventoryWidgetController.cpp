// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"
#include "UI/Widget/Inventory/DonInventorySlotWidget.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	DonPlayerState->GetInventoryComponent()->OnInventoryItemAdded.AddDynamic(this, &UInventoryWidgetController::OnItemAdded);
	DonPlayerState->GetInventoryComponent()->OnInventoryItemAdded.AddDynamic(this, &UInventoryWidgetController::OnItemRemoved);
}

void UInventoryWidgetController::OnItemAdded(FItem Item)
{
	OnInventoryItemAdded.Broadcast(Item);
}

void UInventoryWidgetController::OnItemRemoved(FItem Item)
{
	OnInventoryItemRemoved.Broadcast(Item);
}

FItem UInventoryWidgetController::GetItemInfo(uint8 SlotIndex)
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	return DonPlayerState->GetInventory()[SlotIndex]; 
}

void UInventoryWidgetController::SwapSlotInfo(UDonInventorySlotWidget* DesiredSlot)
{
	FItem CachedItem;
	CachedItem.ItemName = DraggedSlot->ItemName;
	CachedItem.ItemID = DraggedSlot->ItemID;
	CachedItem.Amount = DraggedSlot->Amount;
	CachedItem.Durability = DraggedSlot->Durability;
	CachedItem.Upgrade = DraggedSlot->Upgrade;
	CachedItem.InventorySlotIndex = DesiredSlot->InventorySlotIndex;
	CachedItem.Icon = DraggedSlot->Icon;

	FItem DesiredItem;
	DesiredItem.ItemName = DesiredSlot->ItemName;
	DesiredItem.ItemID = DesiredSlot->ItemID;
	DesiredItem.Amount = DesiredSlot->Amount;
	DesiredItem.Durability = DesiredSlot->Durability;
	DesiredItem.Upgrade = DesiredSlot->Upgrade;
	DesiredItem.InventorySlotIndex = DraggedSlot->InventorySlotIndex;
	DesiredItem.Icon = DesiredSlot->Icon;

	
	// Update Inventory Item Info
	
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	DonPlayerState->GetInventory()[DraggedSlot->InventorySlotIndex] = DesiredItem;
	DonPlayerState->GetInventory()[DesiredSlot->InventorySlotIndex] = CachedItem;

	// End Update Inventory Item Info

	
	DraggedSlot->UpdateSlotInfo(DesiredItem);
	DesiredSlot->UpdateSlotInfo(CachedItem);
	
	DraggedSlot->NotifyUpdateSlot();
	DesiredSlot->NotifyUpdateSlot();

	DraggedSlot = nullptr;
}

void UInventoryWidgetController::AddItem(FName ItemName)
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	DonPlayerState->GetInventoryComponent()->AddItem(DonPlayerState, ItemName);
}

void UInventoryWidgetController::RemoveItem(int32 SlotIndex)
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	FItem ItemToRemove;
	ItemToRemove.InventorySlotIndex = SlotIndex;
	DonPlayerState->GetInventory()[SlotIndex] = ItemToRemove;
}
