// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/DonInventorySlotWidget.h"

#include "Inventory/InventoryComponent.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UDonInventorySlotWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	if (UInventoryWidgetController* InventoryController = Cast<UInventoryWidgetController>(WidgetController))
	{
		InventoryController->GetInventoryComponent()->OnInventoryItemAdded.AddDynamic(this, &UDonInventorySlotWidget::UpdateSlotInfo);
		InventoryController->GetInventoryComponent()->OnInventoryItemRemoved.AddDynamic(this, &UDonInventorySlotWidget::UpdateSlotInfo);
	}
	WidgetControllerSet();
}

void UDonInventorySlotWidget::UpdateSlotInfo(FItem ItemInfo)
{
	if (InventorySlotIndex == -1 || ItemInfo.InventorySlotIndex == InventorySlotIndex)
	{
		InventorySlotIndex = ItemInfo.InventorySlotIndex;
		NotifyUpdateSlot(ItemInfo);
	}
}

void UDonInventorySlotWidget::NotifyUpdateSlot_Implementation(const FItem& Item)
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

void UDonInventorySlotWidget::RemoveItem(FItem Item, int32 AmountToRemove)
{
	Cast<UInventoryWidgetController>(WidgetController)->RemoveItemFromPlayer(Item, AmountToRemove);
}
