// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DonItemLibrary.h"

#include "DonGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"
#include "UI/HUD/DonHUD.h"
#include "UI/WidgetController/DonWidgetController.h"

FItem UDonItemLibrary::FindItemByName(const UObject* WorldContextObject, FName ItemName)
{
	const ADonGameModeBase* DonGameMode = Cast<ADonGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (DonGameMode == nullptr) return FItem();

	for (FItem Item : DonGameMode->ItemInfoAsset->ItemInformation)
	{
		if (Item.ItemName == ItemName)
		{
			return Item;
		}
	}
	
	return FItem();
}

UInventoryWidgetController* UDonItemLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(PC->GetHUD()))
		{
			ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DonHUD->GetInventoryWidgetController(WidgetControllerParams);
		}
	}
	
	return nullptr;
}
