// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/DonGameInstance.h"

#include "Engine/DataTable.h"

void UDonGameInstance::Init()
{
	Super::Init();

	if (!InitDialogueDataTable()) UE_LOG(LogTemp, Warning, TEXT("Cannot initialize Dialogue Data Table"));
	if (!InitQuestDataTable()) UE_LOG(LogTemp, Warning, TEXT("Cannot initialize Quest Data Table"));
}

bool UDonGameInstance::InitDialogueDataTable()
{
	UDataTable* LoadedDialogueTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprints/Data/Dialogue/DT_Dialogue.DT_Dialogue'"));
	if (LoadedDialogueTable == nullptr) return false;
	
	TArray<FDialogue*> AllDialogues;
	LoadedDialogueTable->GetAllRows(TEXT(""), AllDialogues);
	
	for (const FDialogue* Dialogue : AllDialogues)
	{
		DialogueDataTable.FindOrAdd(Dialogue->NPCName).Dialogues.AddUnique(*Dialogue);		
	}
	return true;
}

bool UDonGameInstance::InitQuestDataTable()
{
	UDataTable* LoadedQuestTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprints/Data/Quest/DT_Quest.DT_Quest'"));
	if (LoadedQuestTable == nullptr) return false;
	
	TArray<FQuestRow*> AllQuests;
	LoadedQuestTable->GetAllRows(TEXT(""), AllQuests);
	
	for (const FQuestRow* QuestRow : AllQuests)
	{
		QuestDataTable.FindOrAdd(QuestRow->QuestNPC).Quests.AddUnique(QuestRow->Quest);
	}
	return true;
}