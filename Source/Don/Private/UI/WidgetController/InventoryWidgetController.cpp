// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

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
}

void UInventoryWidgetController::HandleInventoryUpdated(const TArray<FItem>& Inventory)
{
	OnInventoryChanged.Broadcast(Inventory);
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