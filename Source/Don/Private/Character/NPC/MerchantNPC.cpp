// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/MerchantNPC.h"

#include "Character/Component/InteractComponent.h"
#include "GameInstance/DonGameInstance.h"
#include "Inventory/DonItemLibrary.h"

void AMerchantNPC::Interact(APlayerState* TargetPlayerState)
{
	GetInteractComponent()->OpenStore(TargetPlayerState);
}

void AMerchantNPC::BeginPlay()
{
	Super::BeginPlay();

	LoadMerchandise();
	SetLifeSpan(LifeTimer);
}

void AMerchantNPC::RemoveMerchandise(FItem Item, int32 Amount)
{
	for (FItem& ItemToFind : Merchandise)
	{
		if (ItemToFind.ItemName == Item.ItemName)
		{
			ItemToFind.Amount -= Amount;
			ItemToFind.Amount = FMath::Clamp(ItemToFind.Amount, 0, 99);
			OnMerchandiseChanged.Broadcast(Item);
			return;
		}
	}
}

void AMerchantNPC::LoadMerchandise()
{
	for (TTuple<FName, int> ItemInfo : ItemsToAdd)
	{
		FItem Item = UDonItemLibrary::FindItemByName(this, ItemInfo.Key);
		Item.Amount = ItemInfo.Value;
		Item.InventorySlotIndex = Merchandise.Num();
		Merchandise.Add(Item);
	}
	
	if (!bLoadBonusMerchandise) return;
	
	UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(GetGameInstance());
	if (DonGameInstance)
	{
		UDataTable* MerchandiseDT = DonGameInstance->MerchandiseDataTable;
		TArray<FMerchandise*> Merchandises;
		MerchandiseDT->GetAllRows<FMerchandise>("", Merchandises);
		
		for (FMerchandise* Info : Merchandises)
		{
			if (Info->NPCName == NPCName)
			{
				for (int32 Index = 0; Index < Info->Merchandise.Num(); Index++)
				{
					FItem ItemToAdd;
					ItemToAdd = UDonItemLibrary::FindItemByName(this, Info->Merchandise[Index].ItemName);
					ItemToAdd.Amount = Info->Merchandise[Index].Amount;
					ItemToAdd.InventorySlotIndex = Merchandise.Num();

					int32 FoundIndex = Merchandise.Find(ItemToAdd);
					if (FoundIndex >= 0) Merchandise[FoundIndex].Amount += ItemToAdd.Amount;
					else Merchandise.Add(ItemToAdd);
				}
				return;
			}
		}
	}
}
