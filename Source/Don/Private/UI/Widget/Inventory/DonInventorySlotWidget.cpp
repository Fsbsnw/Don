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

bool UDonInventorySlotWidget::UpgradeEquipment(int32 Points)
{
	if (UInventoryWidgetController* IWC = Cast<UInventoryWidgetController>(WidgetController))
	{
		if (!IWC->GetItemInfo(InventorySlotIndex).ItemTag.MatchesTag(FDonGameplayTags::Get().Item_Equippable)) return false;
		if (!IWC->UpgradeArmorItem(InventorySlotIndex, Points)) return false;
		return true;
	}
	return false;
}