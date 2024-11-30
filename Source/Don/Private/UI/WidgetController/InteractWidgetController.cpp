// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InteractWidgetController.h"

#include "Character/Component/InteractComponent.h"
#include "Character/NPC/NPCCharacterBase.h"
#include "Data/Dialogue.h"
#include "Inventory/DonItemLibrary.h"
#include "Player/DonPlayerState.h"

void UInteractWidgetController::SetWidgetControllerParams(UInteractComponent* InInteractComponent, APlayerState* TargetPlayerState)
{
	InteractComponent = InInteractComponent;
	PlayerState = TargetPlayerState;
}

FDialogue UInteractWidgetController::SetCurrentDialogueProgress() const
{
	TMap<ENPCName, FDialogueContainer> CompletedDialogues = Cast<ADonPlayerState>(PlayerState)->CompletedDialogues;
	const ANPCCharacterBase* NPCCharacter = Cast<ANPCCharacterBase>(InteractComponent->GetOwner());
	
	FDialogueContainer* EndedDialoguesForNPC = CompletedDialogues.Find(NPCCharacter->NPCName);
	FDialogue InitialDialogue;

	if (EndedDialoguesForNPC && EndedDialoguesForNPC->Dialogues.Num() > 0)
	{
		for (FDialogue Dialogue : EndedDialoguesForNPC->Dialogues)
		{
			if (Dialogue.DialogueType == EDialogueType::ContinueFromLast)
			{
				InitialDialogue = Dialogue;
			}
		}
	}
	else
	{
		UDonItemLibrary::FindDialogueRow(GetWorld(), InitialDialogue, NPCCharacter->NPCName, EDialogueFlow::Normal, 0, 1);
	}
	
	return InitialDialogue;
}

FDialogue UInteractWidgetController::FindDialogueRow(const ENPCName& NPCName, const EDialogueFlow& DialogueFlow, const int32 Branch, const int32 Progress) const
{
	FDialogue Dialogue;
	UDonItemLibrary::FindDialogueRow(GetWorld(), Dialogue, NPCName, DialogueFlow, Branch ,Progress);
	return Dialogue;
}

void UInteractWidgetController::AddItemToPlayerState(const FItem& Item, int32 Amount)
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		TArray<FItem>& Inventory = DonPlayerState->GetInventory();
		if (Item.ItemType == EItemType::Equipable || Inventory.Contains(Item))
		{
			Inventory.Add(Item);
		}
		else
		{
			int32 ItemIndex = Inventory.Find(Item);
			if (ItemIndex != INDEX_NONE) 
			{
				Inventory[ItemIndex].Amount += Amount;
			}
		}

		
	}
}

void UInteractWidgetController::AddDialogueToPlayerState(const FDialogue& Dialogue)
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		DonPlayerState->CompletedDialogues.FindOrAdd(Dialogue.NPCName).Dialogues.AddUnique(Dialogue);
	}
}

void UInteractWidgetController::AddQuestToPlayerState(FQuest Quest, EQuestState QuestState)
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		Quest.QuestState = QuestState;
		if (!DonPlayerState->PlayerQuests.FindOrAdd(Quest.QuestNPC).Quests.Contains(Quest))
		{
			DonPlayerState->OnQuestListChanged.Broadcast(Quest);
		}
		DonPlayerState->PlayerQuests.FindOrAdd(Quest.QuestNPC).Quests.AddUnique(Quest);
	}
}
