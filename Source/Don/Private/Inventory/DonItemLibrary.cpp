// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DonItemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "DonGameModeBase.h"
#include "DonGameplayTags.h"
#include "Data/LootableActorDataAsset.h"
#include "GameInstance/DonGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/LootableActor.h"
#include "Data/DonItem_EquipmentBase.h"
#include "Data/DonItem_PotionBase.h"
#include "Data/ItemAsset.h"
#include "Data/ItemConsumableAsset.h"
#include "Data/ItemEquipmentAsset.h"
#include "Data/ItemStructs.h"

FItem UDonItemLibrary::FindItemByName(const UObject* WorldContextObject, FName ItemName)
{
	const UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance == nullptr || DonGameInstance->ItemInfoAsset == nullptr) return FItem();

	return DonGameInstance->ItemInfoAsset->FindItemByName(ItemName);
}

FItemEquipmentInfo UDonItemLibrary::FindItemEquipmentByName(const UObject* WorldContextObject, FName ItemName)
{
	const UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance == nullptr || DonGameInstance->ItemEquipmentAsset == nullptr) return FItemEquipmentInfo();

	return DonGameInstance->ItemEquipmentAsset->FindItemEquipmentByName(ItemName);
}

FItemConsumableInfo UDonItemLibrary::FindItemConsumableByName(const UObject* WorldContextObject, FName ItemName)
{
	const UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance == nullptr || DonGameInstance->ItemConsumableAsset == nullptr) return FItemConsumableInfo();

	return DonGameInstance->ItemConsumableAsset->FindItemConsumableByName(ItemName);
}

UDonItemBase* UDonItemLibrary::CreateItemObjectByTag(const UObject* WorldContextObject, FGameplayTag Tag)
{
	FDonGameplayTags DonGameplayTags = FDonGameplayTags::Get();
	if (Tag.MatchesTag(DonGameplayTags.Item_Consumable))
	{
		return NewObject<UDonItem_PotionBase>(WorldContextObject->GetWorld());
	}
	if (Tag.MatchesTag(DonGameplayTags.Item_Equippable))
	{
		return NewObject<UDonItem_EquipmentBase>(WorldContextObject->GetWorld());
	}
	return NewObject<UDonItemBase>(WorldContextObject->GetWorld());
}

FEquipmentAttributes UDonItemLibrary::FindEquipmentAttributes(const UObject* WorldContextObject, FName EquipmentName)
{
	UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance)
	{
		if (FEquipmentAttributes* Equipment = DonGameInstance->EquipmentDataTable.Find(EquipmentName)) return *Equipment;		
	}
	return FEquipmentAttributes();
}

void UDonItemLibrary::SpawnLootableXP(const UObject* WorldContextObject, int32 Amount, FVector SpawnLocation, FRotator SpawnRotation)
{
	const FDonGameplayTags& GameplayTags = FDonGameplayTags::Get();
	
	ADonGameModeBase* GameModeBase = Cast<ADonGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));;
	if (GameModeBase && GameModeBase->LootableActorAsset && GameModeBase->LootableActorAsset->LootableXP)
	{
		TSubclassOf<ALootableActor> LootableRewardClass = GameModeBase->LootableActorAsset->LootableXP;
		SpawnLocation.Z = 0.f;
		ALootableActor* LootableReward = WorldContextObject->GetWorld()->SpawnActor<ALootableActor>(LootableRewardClass, SpawnLocation, SpawnRotation);
		if (LootableReward)
		{
			LootableReward->LootType = GameplayTags.Attributes_Meta_IncomingXP;
			LootableReward->LootAmount = Amount;
		}
	}
}

void UDonItemLibrary::SpawnLootableMoney(const UObject* WorldContextObject, int32 Amount, int32 Quantity, FVector SpawnLocation, FRotator SpawnRotation)
{
	const FDonGameplayTags& GameplayTags = FDonGameplayTags::Get();
	
	ADonGameModeBase* GameModeBase = Cast<ADonGameModeBase>(WorldContextObject->GetWorld()->GetAuthGameMode());
	if (GameModeBase && GameModeBase->LootableActorAsset && GameModeBase->LootableActorAsset->LootableMoney)
	{
		TSubclassOf<ALootableActor> LootableRewardClass = GameModeBase->LootableActorAsset->LootableMoney;
		for (int32 i = 0; i < Quantity; i++)
		{
			SpawnLocation.X += FMath::FRandRange(-100.f, 100.f);
			SpawnLocation.Y += FMath::FRandRange(-100.f, 100.f);
			SpawnLocation.Z = 0.f;
			ALootableActor* LootableReward = WorldContextObject->GetWorld()->SpawnActor<ALootableActor>(LootableRewardClass, SpawnLocation, SpawnRotation);
			if (LootableReward)
			{
				int32 AdjustedAmount = Amount / 10;
				LootableReward->LootType = GameplayTags.Attributes_Meta_IncomingMoney;
				LootableReward->LootAmount = Amount + FMath::RandRange(-AdjustedAmount, AdjustedAmount);
			}
		}
	}
}

void UDonItemLibrary::SpawnLootableItem(const UObject* WorldContextObject, TArray<FItemLootableInfo> Items, FVector SpawnLocation, FRotator SpawnRotation, float ItemDropRate)
{
	if (FMath::RandRange(1, 100) > ItemDropRate * 100) return;
	
	const FDonGameplayTags& GameplayTags = FDonGameplayTags::Get();
	
	ADonGameModeBase* GameModeBase = Cast<ADonGameModeBase>(WorldContextObject->GetWorld()->GetAuthGameMode());
	if (GameModeBase && GameModeBase->LootableActorAsset && GameModeBase->LootableActorAsset->LootableItem)
	{
		TSubclassOf<ALootableActor> LootableRewardClass = GameModeBase->LootableActorAsset->LootableItem;
		
		SpawnLocation.X += FMath::FRandRange(-100.f, 100.f);
		SpawnLocation.Y += FMath::FRandRange(-100.f, 100.f);
		SpawnLocation.Z = 0.f;
		ALootableActor* LootableReward = WorldContextObject->GetWorld()->SpawnActor<ALootableActor>(LootableRewardClass, SpawnLocation, SpawnRotation);
		if (LootableReward)
		{
			LootableReward->LootType = GameplayTags.Item_Miscellaneous;
		}
	}
}