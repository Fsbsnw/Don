// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/DonInventorySlotWidget.h"

#include "UI/WidgetController/InventoryWidgetController.h"

void UDonInventorySlotWidget::UpdateSlotInfo(FItem ItemInfo)
{
	ItemName = ItemInfo.ItemName;
	ItemID = ItemInfo.ItemID;
	Amount = ItemInfo.Amount;
	Durability = ItemInfo.Durability;
	Upgrade = ItemInfo.Upgrade;
	InventorySlotIndex = ItemInfo.InventorySlotIndex;
	Icon = ItemInfo.Icon;
}

void UDonInventorySlotWidget::NotifyUpdateSlot_Implementation()
{
}

void UDonInventorySlotWidget::CachingDraggedSlotInfo(UDonInventorySlotWidget* InSlotWidget)
{
	Cast<UInventoryWidgetController>(WidgetController)->CachingDraggedSlot(InSlotWidget);
}

void UDonInventorySlotWidget::SwapSlotInfo(UDonInventorySlotWidget* InSlotWidget)
{
	Cast<UInventoryWidgetController>(WidgetController)->SwapSlotInfo(InSlotWidget);
}

void UDonInventorySlotWidget::RemoveItem(int32 SlotIndex)
{
	Cast<UInventoryWidgetController>(WidgetController)->RemoveItem(SlotIndex);
}
