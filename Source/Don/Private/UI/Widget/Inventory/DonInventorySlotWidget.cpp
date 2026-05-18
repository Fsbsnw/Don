// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Inventory/DonInventorySlotWidget.h"

#include "DonGameplayTags.h"
#include "Data/ItemStructs.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UDonInventorySlotWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	if (UInventoryWidgetController* InventoryController = Cast<UInventoryWidgetController>(WidgetController))
	{
		InventoryController->OnInventoryChanged.AddDynamic(this, &UDonInventorySlotWidget::HandleInventoryUpdated);
	}
	OnWidgetControllerSet();
}

void UDonInventorySlotWidget::HandleInventoryUpdated(const TArray<FItem>& Inventory)
{
	NotifyUpdateSlot(Inventory[InventorySlotIndex]);
}