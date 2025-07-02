// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DonItemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "DonAbilityTypes.h"
#include "DonGameModeBase.h"
#include "DonGameplayTags.h"
#include "Data/LootableActorDataAsset.h"
#include "GameInstance/DonGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"
#include "UI/HUD/DonHUD.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "UI/WidgetController/QuestListWidgetController.h"
#include "Actor/LootableActor.h"
#include "Character/DonCharacterBase.h"
#include "Data/DonItem_EquipmentBase.h"
#include "Data/DonItem_PotionBase.h"
#include "Data/ItemConsumableAsset.h"
#include "Data/ItemEquipmentAsset.h"
#include "Data/ItemStructs.h"
#include "Data/NPCInfo.h"

FItem UDonItemLibrary::FindItemByName(const UObject* WorldContextObject, FName ItemName)
{
	const ADonGameModeBase* DonGameMode = Cast<ADonGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (DonGameMode == nullptr || DonGameMode->ItemInfoAsset == nullptr) return FItem();

	return DonGameMode->ItemInfoAsset->FindItemByName(ItemName);
}

FItemEquipmentInfo UDonItemLibrary::FindItemEquipmentByName(const UObject* WorldContextObject, FName ItemName)
{
	const ADonGameModeBase* DonGameMode = Cast<ADonGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (DonGameMode == nullptr || DonGameMode->ItemEquipmentAsset == nullptr) return FItemEquipmentInfo();

	return DonGameMode->ItemEquipmentAsset->FindItemEquipmentByName(ItemName);
}

FItemConsumableInfo UDonItemLibrary::FindItemConsumableByName(const UObject* WorldContextObject, FName ItemName)
{
	const ADonGameModeBase* DonGameMode = Cast<ADonGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (DonGameMode == nullptr || DonGameMode->ItemConsumableAsset == nullptr) return FItemConsumableInfo();

	return DonGameMode->ItemConsumableAsset->FindItemConsumableByName(ItemName);
}

UDonItemBase* UDonItemLibrary::CreateItemObjectByTag(const UObject* WorldContextObject, FGameplayTag Tag)
{
	FDonGameplayTags DonGameplayTags = FDonGameplayTags::Get();
	if (Tag.MatchesTag(DonGameplayTags.Item_Consumable))
	{
		return NewObject<UDonItem_PotionBase>(WorldContextObject->GetWorld());
	}
	else if (Tag.MatchesTag(DonGameplayTags.Item_Equippable))
	{
		return NewObject<UDonItem_EquipmentBase>(WorldContextObject->GetWorld());
	}
	return NewObject<UDonItemBase>(WorldContextObject->GetWorld());
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

UAttributeMenuWidgetController* UDonItemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(PC->GetHUD()))
		{
			ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DonHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}


FCharacterClassInfo UDonItemLibrary::FindCharacterClassInfo(const UObject* WorldContextObject, ECharacterClass CharacterClass)
{
	UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance && DonGameInstance->CharacterClassDataTable)
	{
		UDataTable* CharacterDT = DonGameInstance->CharacterClassDataTable;
		TArray<FCharacterClassInfo*> CharacterClasses; 
		CharacterDT->GetAllRows(TEXT(""), CharacterClasses);
		for (FCharacterClassInfo* Character : CharacterClasses)
		{
			if (Character->CharacterClass == CharacterClass) return *Character;
		}
	}
	return FCharacterClassInfo();
}

UAbilityInfo* UDonItemLibrary::GetAbilityInfo(const UObject* WorldContextObject, ECharacterClass CharacterClass)
{
	UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance && DonGameInstance->AbilityInfo)
	{
		// Check Character Class
		
		return DonGameInstance->AbilityInfo;
	}
	return nullptr;
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

void UDonItemLibrary::SpawnLootableItem(const UObject* WorldContextObject, TArray<FLootableItem> Items, FVector SpawnLocation, FRotator SpawnRotation, float ItemDropRate)
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

UQuestListWidgetController* UDonItemLibrary::GetQuestListWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(PC->GetHUD()))
		{
			ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DonHUD->GetQuestListWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

bool UDonItemLibrary::FindQuestRow(const UObject* WorldContextObject, FQuest& OutQuest, ENPCName NPCName, FString QuestTitle)
{
	if (const UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		TMap<ENPCName, FQuestContainer> QuestTable = DonGameInstance->QuestDataTable;
		
		FQuestContainer* QuestsForNPC = QuestTable.Find(NPCName);
		if (QuestsForNPC == nullptr || QuestsForNPC->Quests.Num() == 0) return false;
		
		for (FQuest Quest : QuestsForNPC->Quests)
		{
			if (Quest.QuestNPC == NPCName && Quest.QuestTitle == QuestTitle)
			{
				OutQuest = Quest;
				return true;
			}
		}
	}	
	return false;
}

bool UDonItemLibrary::FindDialogueRow(const UObject* WorldContextObject, FDialogue& OutDialogue, FDonDialogueContext& DialogueContext)
{
	if (const UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		TMap<ENPCName, FDialogueContainer> DialogueTable = DonGameInstance->DialogueDataTable;
	
		const ENPCName NPCName = DialogueContext.NPCName;
		const int32 Chapter = DialogueContext.Chapter;
		const FString Topic = DialogueContext.Topic;
		const FString Branch = DialogueContext.Branch;
		const int32 Progress = DialogueContext.Progress;
		
		FDialogueContainer* DialoguesForNPC = DialogueTable.Find(NPCName);
		if (DialoguesForNPC == nullptr || DialoguesForNPC->Dialogues.Num() == 0) return false;
		
		for (FDialogue Dialogue : DialoguesForNPC->Dialogues)
		{
			if (Dialogue.Chapter == Chapter &&
				Dialogue.Topic == Topic &&
				Dialogue.Branch == Branch &&
				Dialogue.Progress == Progress)
			{
				OutDialogue = Dialogue;
				return true;
			}
		}
	}	
	return false;
}

UTexture2D* UDonItemLibrary::GetNPCImage(const UObject* WorldContextObject, ENPCName NPCName)
{
	ADonGameModeBase* GameModeBase = Cast<ADonGameModeBase>(WorldContextObject->GetWorld()->GetAuthGameMode());
	if (GameModeBase && GameModeBase->NPCInfoAsset)
	{
		return GameModeBase->NPCInfoAsset->GetNPCImage(NPCName);
	}
	return nullptr;
}


FGameplayEffectContextHandle UDonItemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FDonGameplayTags& GameplayTags = FDonGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	if (FDonGameplayEffectContext* DonEffectContext = static_cast<FDonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DonEffectContext->SetKnockbackForce(DamageEffectParams.KnockbackForce);
	}
	// SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	// SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);
	//
	// SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	// SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	// SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	// SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	// UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	// UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	// UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	// UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}
