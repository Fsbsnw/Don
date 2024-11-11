// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/DonGameInstance.h"

#include "Engine/DataTable.h"

void UDonGameInstance::Init()
{
	Super::Init();

	UDataTable* LoadedDialogueTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprints/Data/Dialogue/DT_Dialogue.DT_Dialogue'"));
	if (LoadedDialogueTable == nullptr) return;
	
	TArray<FDialogue*> AllDialogues;
	LoadedDialogueTable->GetAllRows(TEXT(""), AllDialogues);
	
	for (FDialogue* Dialogue : AllDialogues)
	{
		DialogueDataTable.FindOrAdd(Dialogue->DialogueID).Dialogues.AddUnique(*Dialogue);		
	}
}
