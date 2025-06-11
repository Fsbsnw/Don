// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/StoreWidgetController.h"

#include "Character/Component/InteractComponent.h"
#include "Character/NPC/MerchantNPC.h"
#include "Character/NPC/NPCCharacterBase.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"

void UStoreWidgetController::SetWidgetControllerParams(UInteractComponent* InInteractComponent,	APlayerState* TargetPlayerState)
{
	InteractComponent = InInteractComponent;
	PlayerState = TargetPlayerState;
}

void UStoreWidgetController::BindCallbacksToDependencies()
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		DonPlayerState->GetInventoryComponent()->OnInventoryItemSold.AddDynamic(this, &UStoreWidgetController::PlayerSellItem);
	}
}

void UStoreWidgetController::ResetInteractWidgets()
{
	InteractComponent->ResetWidgets();
}

TArray<FItem> UStoreWidgetController::GetNPCMerchandise()
{
	AMerchantNPC* MerchantNPC = Cast<AMerchantNPC>(InteractComponent->GetOwner());
	if (MerchantNPC)
	{
		return MerchantNPC->GetMerchandise();
	}
	return TArray<FItem>();
}

void UStoreWidgetController::NPCSellMerchandise(int32 SlotIndex)
{
	if (AMerchantNPC* MerchantNPC = Cast<AMerchantNPC>(InteractComponent->GetOwner()))
	{
		FItem MerchandiseToSell = MerchantNPC->GetMerchandise()[SlotIndex];
		MerchantNPC->RemoveMerchandise(MerchandiseToSell, 1);
		PlayerBuyItem(MerchandiseToSell, 1);
		// Broadcast delegate for UI blueprint
		OnMerchandiseUpdate.Broadcast(GetNPCMerchandise()[SlotIndex]);
	}
}

bool UStoreWidgetController::PlayerCanAffordItem(int32 Cost)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		return DonPlayerState->GetInventoryComponent()->CanAffordItem(Cost);
	}
	return false;
}

void UStoreWidgetController::PlayerBuyItem(FItem Item, int32 Amount)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		DonPlayerState->GetInventoryComponent()->AddItem(Item, Amount);
		DonPlayerState->AddToMoney(-Item.ItemPrice);
	}
}

void UStoreWidgetController::PlayerSellItem(int32 SlotIndex)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		FItem ItemToSell = DonPlayerState->GetInventoryComponent()->GetInventory()[SlotIndex];
		DonPlayerState->GetInventoryComponent()->RemoveItem(ItemToSell, SlotIndex);

		int32 AdjustedPrice = ItemToSell.ItemPrice;

		if (ANPCCharacterBase* NPC = Cast<ANPCCharacterBase>(InteractComponent->GetOwner()))
		{
			if (ItemToSell.ItemType == EItemType::Equipable)
			{
				AdjustedPrice = ItemToSell.ItemPrice * NPC->EquipmentPriceRate;
			}
			else if (ItemToSell.ItemType == EItemType::Consumable)
			{
				AdjustedPrice = ItemToSell.ItemPrice * NPC->ConsumablePriceRate;
			}
			else
			{
				AdjustedPrice = ItemToSell.ItemPrice * NPC->NormalPriceRate;
			}
		}
		
		DonPlayerState->AddToMoney(AdjustedPrice);
	}
}

AActor* UStoreWidgetController::GetNPCCharacter()
{
	return InteractComponent->GetOwner() ? InteractComponent->GetOwner() : nullptr;
}
