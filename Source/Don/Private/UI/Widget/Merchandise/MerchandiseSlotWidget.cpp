// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Merchandise/MerchandiseSlotWidget.h"

#include "Character/NPC/MerchantNPC.h"
#include "UI/WidgetController/StoreWidgetController.h"

void UMerchandiseSlotWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;

	if (UStoreWidgetController* StoreWidgetController = Cast<UStoreWidgetController>(WidgetController))
	{
		if (AMerchantNPC* MerchantNPC = Cast<AMerchantNPC>(StoreWidgetController->GetNPCCharacter()))
		{
			MerchantNPC->OnMerchandiseChanged.AddDynamic(this, &UMerchandiseSlotWidget::UpdateSlotInfo);
		}
	}
	
	WidgetControllerSet();
}

void UMerchandiseSlotWidget::UpdateSlotInfo(FItem ItemInfo)
{
	if (InventorySlotIndex == -1 || ItemInfo.InventorySlotIndex == InventorySlotIndex)
	{
		InventorySlotIndex = ItemInfo.InventorySlotIndex;
		NotifyUpdateSlot(ItemInfo);
	}
}

void UMerchandiseSlotWidget::NotifyUpdateSlot_Implementation(const FItem& Item)
{
}

void UMerchandiseSlotWidget::RemoveMerchandise(FItem Item, int32 AmountToRemove)
{
	UStoreWidgetController* StoreWidgetController = Cast<UStoreWidgetController>(WidgetController);
	if (StoreWidgetController)
	{
		StoreWidgetController->PlayerBuyItem(Item, AmountToRemove);
		StoreWidgetController->NPCSellMerchandise(Item.InventorySlotIndex);
	}
}

