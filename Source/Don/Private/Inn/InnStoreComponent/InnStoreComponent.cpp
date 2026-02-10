// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/InnStoreComponent/InnStoreComponent.h"

#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"

UInnStoreComponent::UInnStoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInnStoreComponent::InitializeStoreMerchandise()
{
	for (const FMerchandiseData& Data : InitialMerchandise)
	{
		FItem Item = UDonItemLibrary::FindItemByName(this, Data.ItemName);
		Item.Amount = Data.ItemAmount;

		InnMerchandise.Add(Item);
	}
}

bool UInnStoreComponent::SellMerchandise(int32 ItemIndex)
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(GetOwner());
	if (!InnMerchandise.IsValidIndex(ItemIndex) || InnMerchandise[ItemIndex].Amount <= 0) return false;
	
	const int32 ItemPrice = InnMerchandise[ItemIndex].ItemPrice;
	if (DonPlayerState->GetMoney() < ItemPrice) return false;
	
	DonPlayerState->AddToMoney(-ItemPrice);
	DonPlayerState->GetInventoryComponent()->AddItem(InnMerchandise[ItemIndex], 1);
	
	if (InnMerchandise[ItemIndex].Amount > 1)
	{
		--InnMerchandise[ItemIndex].Amount;
	}
	else
	{
		InnMerchandise.RemoveAt(ItemIndex);
	}
	OnInnMerchandiseChanged.Broadcast(InnMerchandise);
	return true;
}
