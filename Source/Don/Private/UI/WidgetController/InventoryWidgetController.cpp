// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "DonGameplayTags.h"
#include "Character/Player/DonCharacter.h"
#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"
#include "UI/Widget/Inventory/DonInventorySlotWidget.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	DonPlayerState->OnMoneyChangedDelegate.AddUObject(this, &UInventoryWidgetController::OnMoneyAdded);
	DonPlayerState->OnMemoryFragmentChangedDelegate.AddUObject(this, &UInventoryWidgetController::OnMemoryFragmentAdded);
}

void UInventoryWidgetController::BroadcastInitialValues()
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		OnMoneyChanged.Broadcast(DonPlayerState->GetMoney());
		OnMemoryFragmentChanged.Broadcast(DonPlayerState->GetMemoryFragment());
	}
}

void UInventoryWidgetController::HandleSlotSellEvent(int32 SlotIndex)
{
	GetInventoryComponent()->OnRequestSellItem(SlotIndex);
}

UInventoryComponent* UInventoryWidgetController::GetInventoryComponent()
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		return DonPlayerState->GetInventoryComponent();
	}
	return nullptr;
}

void UInventoryWidgetController::OnItemAdded(FItem Item)
{
	OnInventoryItemAdded.Broadcast(Item);
}

void UInventoryWidgetController::OnItemRemoved(FItem Item)
{
	OnInventoryItemRemoved.Broadcast(Item);
}

FItem UInventoryWidgetController::GetItemInfo(int32 SlotIndex)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		if (SlotIndex != -1) return DonPlayerState->GetInventoryComponent()->GetInventory()[SlotIndex]; 
	}
	return FItem();
}

void UInventoryWidgetController::SwapSlotInfo(UDonInventorySlotWidget* DesiredSlot)
{
	if (DraggedSlot->InventorySlotIndex == -1 || DesiredSlot->InventorySlotIndex == -1) return;
	
	if (ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState))
	{
		int32 DraggedIndex = DraggedSlot->InventorySlotIndex;
		int32 DesiredIndex = DesiredSlot->InventorySlotIndex;

		DonPlayerState->GetInventoryComponent()->SwapInventoryItems(DraggedIndex, DesiredIndex);
		
		TArray<FItem>& Inventory = DonPlayerState->GetInventoryComponent()->GetInventory();
		
		DraggedSlot->UpdateSlotInfo(Inventory[DraggedIndex]);
		DesiredSlot->UpdateSlotInfo(Inventory[DesiredIndex]);
	}

	DraggedSlot = nullptr;
}

void UInventoryWidgetController::AddItemToPlayer(FItem Item, int32 Amount)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		DonPlayerState->GetInventoryComponent()->AddItem(Item, Amount);
	}
}

void UInventoryWidgetController::RemoveItemFromPlayer(FItem Item, int32 Amount)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		DonPlayerState->GetInventoryComponent()->RemoveItem(Item.InventorySlotIndex, Amount);
	}
}

void UInventoryWidgetController::UnequipAllItems()
{
	if (APawn* ControlledPawn = Cast<APawn>(PlayerController->GetPawn()))
	{
		if (ControlledPawn && ControlledPawn->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_UnequipAllItems(ControlledPawn);
		}
	}
}

void UInventoryWidgetController::UseItem(int32 SlotIndex)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		DonPlayerState->GetInventoryComponent()->UseItem(SlotIndex);
	}
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

void UInventoryWidgetController::OnMoneyAdded(int32 Money)
{
	OnMoneyChanged.Broadcast(Money);
}

void UInventoryWidgetController::OnMemoryFragmentAdded(int32 MemoryFragment)
{
	OnMemoryFragmentChanged.Broadcast(MemoryFragment);
}

void UInventoryWidgetController::AddMemoryFragment(int32 InMemoryFragment)
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		DonPlayerState->AddToMemoryFragment(InMemoryFragment);
	}
}