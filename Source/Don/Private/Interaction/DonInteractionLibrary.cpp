// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/DonInteractionLibrary.h"

#include "DonGameModeBase.h"
#include "Data/NPCInfo.h"
#include "GameInstance/DonGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"
#include "UI/HUD/DonHUD.h"
#include "UI/WidgetController/DonWidgetController.h"


bool UDonInteractionLibrary::FindQuestRow(const UObject* WorldContextObject, FQuest& OutQuest, ENPCName NPCName, FString QuestTitle)
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

bool UDonInteractionLibrary::FindDialogueRow(const UObject* WorldContextObject, FDialogue& OutDialogue, FDonDialogueContext& DialogueContext)
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

UTexture2D* UDonInteractionLibrary::GetNPCImage(const UObject* WorldContextObject, ENPCName NPCName)
{
	ADonGameModeBase* GameModeBase = Cast<ADonGameModeBase>(WorldContextObject->GetWorld()->GetAuthGameMode());
	if (GameModeBase && GameModeBase->NPCInfoAsset)
	{
		return GameModeBase->NPCInfoAsset->GetNPCImage(NPCName);
	}
	return nullptr;
}
