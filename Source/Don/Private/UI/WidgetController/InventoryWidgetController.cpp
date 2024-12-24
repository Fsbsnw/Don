// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"
#include "UI/Widget/Inventory/DonInventorySlotWidget.h"

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	DonPlayerState->OnMoneyChangedDelegate.AddUObject(this, &UInventoryWidgetController::OnMoneyAdded);
}

void UInventoryWidgetController::BroadcastInitialValues()
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		OnMoneyChanged.Broadcast(DonPlayerState->GetMoney()); 
	}
}

void UInventoryWidgetController::InitAndLoadInventory()
{
	if (SlotWidgetClass == nullptr) return;
	
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		for (int32 i = 0; i < DonPlayerState->GetInventoryComponent()->MaxItemSlots; i++)
		{
			UDonInventorySlotWidget* SlotWidget = CreateWidget<UDonInventorySlotWidget>(GetWorld(), SlotWidgetClass);
			SlotWidget->SetWidgetController(this);
		}		
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
		DonPlayerState->GetInventoryComponent()->RemoveItem(Item, Amount);
	}
}

void UInventoryWidgetController::OnMoneyAdded(int32 Money)
{
	OnMoneyChanged.Broadcast(Money);
}
