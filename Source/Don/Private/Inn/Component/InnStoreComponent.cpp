// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Component/InnStoreComponent.h"

#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"

UInnStoreComponent::UInnStoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInnStoreComponent::SavePlayerData(FPlayerSaveData& Data)
{
	Data.InnMerchandise = InnMerchandise;
	Data.InnUpgradeCost = InnUpgradeCost;
	Data.InnUpgradeCrystal = InnUpgradeCrystal;
	
	Data.HiringChefCount = HiringChefCount;
	Data.ChefHiringCost = ChefHiringCost;
	Data.ChefHiringCrystal = ChefHiringCrystal;
}

void UInnStoreComponent::LoadPlayerData(const FPlayerSaveData& InData)
{
	InnMerchandise = InData.InnMerchandise;
	
	InnUpgradeLevel = InData.InnLevel;
	InnUpgradeCost = InData.InnUpgradeCost;
	InnUpgradeCrystal = InData.InnUpgradeCrystal;

	HiringChefCount = InData.HiringChefCount;
	ChefHiringCost = InData.ChefHiringCost;
	ChefHiringCrystal = InData.ChefHiringCrystal;

	if (InnMerchandise.IsEmpty()) InitializeStoreMerchandise();
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

	if (InnMerchandise[ItemIndex].ItemName == FName("DevilPotion"))
	{
		OnDevilPotionPurchased.Broadcast();
	}
	
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

bool UInnStoreComponent::UpgradeInn(bool bTargetIsTable)
{
	ADonPlayerState* DPS = CastChecked<ADonPlayerState>(GetOwner());

	TArray<FItem> CostItems;
	FItem ItemToCost = UDonItemLibrary::FindItemByName(this, FName("Upgrade Crystal"));

	if (bTargetIsTable)
	{
		ItemToCost.Amount = InnUpgradeCrystal;
		CostItems.Add(ItemToCost);

		if (DPS->GetMoney() < InnUpgradeCost * InnUpgradeLevel ||
			!DPS->GetInventoryComponent()->HasEnoughItems(CostItems) ||
			InnUpgradeLevel >= 6) return false;

		DPS->GetInventoryComponent()->SellItem(ItemToCost, ItemToCost.Amount);
		DPS->AddToMoney(-InnUpgradeCost * InnUpgradeLevel);
		InnUpgradeCrystal += 5;
		++InnUpgradeLevel;
		OnUpgradeRequested.Broadcast(bTargetIsTable);
		OnTableUpgradeChanged.Broadcast(GetTableUpgradeResources());
	}
	else
	{
		ItemToCost.Amount = ChefHiringCrystal;
		CostItems.Add(ItemToCost);

		if (DPS->GetMoney() < ChefHiringCost * HiringChefCount ||
			!DPS->GetInventoryComponent()->HasEnoughItems(CostItems) ||
			HiringChefCount >= 3) return false;

		DPS->GetInventoryComponent()->SellItem(ItemToCost, ItemToCost.Amount);
		DPS->AddToMoney(-ChefHiringCost * HiringChefCount);
		ChefHiringCrystal += 10;
		++HiringChefCount;
		OnUpgradeRequested.Broadcast(bTargetIsTable);
		OnChefUpgradeChanged.Broadcast(GetChefUpgradeResources());
	}
	return true;
}

FInnUpgradeResources UInnStoreComponent::GetTableUpgradeResources()
{
	FInnUpgradeResources Resources;
	Resources.RequiredLevel = InnUpgradeLevel + 1;
	Resources.RequiredCost = InnUpgradeCost * InnUpgradeLevel;
	Resources.RequiredCrystal = InnUpgradeCrystal;
	return Resources;
}

FInnUpgradeResources UInnStoreComponent::GetChefUpgradeResources()
{
	FInnUpgradeResources Resources;
	Resources.RequiredLevel = HiringChefCount + 1;
	Resources.RequiredCost = ChefHiringCost * HiringChefCount;
	Resources.RequiredCrystal = ChefHiringCrystal;
	return Resources;
}
