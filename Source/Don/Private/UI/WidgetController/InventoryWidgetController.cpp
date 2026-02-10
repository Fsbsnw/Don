// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Inn/InnManagerComponent/InnManagerComponent.h"
#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	DonPlayerState->GetInventoryComponent()->OnInventoryChanged.AddDynamic(this, &UInventoryWidgetController::HandleInventoryUpdated);
}

void UInventoryWidgetController::BroadcastInitialValues()
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	OnMoneyChanged.Broadcast(DonPlayerState->GetMoney());
	OnMemoryFragmentChanged.Broadcast(DonPlayerState->GetMemoryFragment());
}

void UInventoryWidgetController::HandleInventoryUpdated(const TArray<FItem>& Inventory)
{
	OnInventoryChanged.Broadcast(Inventory);
}

void UInventoryWidgetController::HandleSlotSellEvent(int32 SlotIndex)
{
	GetInventoryComponent()->OnRequestSellItem(SlotIndex);
}

UInventoryComponent* UInventoryWidgetController::GetInventoryComponent()
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	return DonPlayerState->GetInventoryComponent();
}

FItem UInventoryWidgetController::GetItemInfo(int32 SlotIndex)
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);

	return DonPlayerState->GetInventoryComponent()->GetInventory()[SlotIndex];
}

void UInventoryWidgetController::SwapSlotInfo(int32 FromIndex, int32 ToIndex)
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	DonPlayerState->GetInventoryComponent()->SwapInventoryItems(FromIndex, ToIndex);
}

void UInventoryWidgetController::AddItemToPlayer(FItem Item, int32 Amount)
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	DonPlayerState->GetInventoryComponent()->AddItem(Item, Amount);
}

void UInventoryWidgetController::UseItem(int32 SlotIndex)
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	DonPlayerState->GetInventoryComponent()->UseItem(SlotIndex);
}

bool UInventoryWidgetController::UpgradeArmorItem(int32 SlotIndex, int32 Points)
{
	FItem Item = UDonItemLibrary::FindItemByName(this, FName("Upgrade Crystal"));
	Item.Amount = 1;
	TArray<FItem> Items;
	Items.Add(Item);
	
	if (GetInventoryComponent()->HasEnoughItems(Items))
	{
		GetInventoryComponent()->UpgradeArmorItem(SlotIndex, Points);
		const int32 Index = GetInventoryComponent()->FindItemInInventory(Item);
		if (Index != INDEX_NONE)
		{
			GetInventoryComponent()->RemoveItem(Index, Item.Amount);
			return true;
		}
	}
	return false;
}