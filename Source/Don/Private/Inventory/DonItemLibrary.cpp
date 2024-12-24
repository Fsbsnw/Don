// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DonItemLibrary.h"

#include "DonGameModeBase.h"
#include "GameInstance/DonGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"
#include "UI/HUD/DonHUD.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "UI/WidgetController/QuestListWidgetController.h"

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
