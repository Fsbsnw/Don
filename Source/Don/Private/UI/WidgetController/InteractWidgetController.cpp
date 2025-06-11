// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InteractWidgetController.h"

#include "Character/Component/InteractComponent.h"
#include "Character/NPC/NPCCharacterBase.h"
#include "Data/Dialogue.h"
#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"

void UInteractWidgetController::SetWidgetControllerParams(UInteractComponent* InInteractComponent, APlayerState* TargetPlayerState)
{
	InteractComponent = InInteractComponent;
	PlayerState = TargetPlayerState;
}

void UInteractWidgetController::ResetInteractWidgets()
{
	InteractComponent->ResetWidgets();
}

bool UInteractWidgetController::HasCompletableQuestsInNPC(TArray<FQuest>& OutQuests)
{
	const ANPCCharacterBase* NPC = Cast<ANPCCharacterBase>(InteractComponent->GetOwner());
	if (NPC)
	{
		if (NPC->CompletableQuests.Num() > 0)
		{
			OutQuests = NPC->CompletableQuests;
			return true;
		}
	}
	return false;
}

bool UInteractWidgetController::HasCompletedQuestInPlayerState(FObjective& Objective) const
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		return DonPlayerState->IsQuestConditionMet(FString("Normal"), Objective);
	}	
	return false;
}

bool UInteractWidgetController::HasDialogueInPlayerState(FObjective& Objective)
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		return DonPlayerState->IsQuestConditionMet(FString("Normal"), Objective);
	}
	return false;
}

bool UInteractWidgetController::HasResponseOptionsInDialogue() const
{
	return CurrentDialogue.ResponseOptions.Num() > 0; 
}

FDialogue UInteractWidgetController::FindCurrentDialogueProgress() const
{
	TMap<ENPCName, FDialogueContainer> CompletedDialogues = Cast<ADonPlayerState>(PlayerState)->CompletedDialogues;
	const ANPCCharacterBase* NPCCharacter = Cast<ANPCCharacterBase>(InteractComponent->GetOwner());
	
	FDialogueContainer* EndedDialoguesForNPC = CompletedDialogues.Find(NPCCharacter->NPCName);
	FDialogue InitialDialogue;

	if (EndedDialoguesForNPC && EndedDialoguesForNPC->Dialogues.Num() > 0)
	{
		for (FDialogue Dialogue : EndedDialoguesForNPC->Dialogues)
		{
			if (Dialogue.Type == EDialogueType::ContinueFromLast)
			{
				InitialDialogue = Dialogue;
			}
		}
	}
	else
	{
		FDonDialogueContext DialogueContext(NPCCharacter->NPCName, 1, "", "Default", 1);
		UDonItemLibrary::FindDialogueRow(GetWorld(), InitialDialogue, DialogueContext);
	}
	return InitialDialogue;
}

FDialogue UInteractWidgetController::FindDialogueRow(FDonDialogueContext& DialogueContext) const
{
	FDialogue Dialogue;
	UDonItemLibrary::FindDialogueRow(GetWorld(), Dialogue, DialogueContext);
	return Dialogue;
}

FQuest UInteractWidgetController::FindCompletableQuest(const FString QuestTitle) const
{
	const ANPCCharacterBase* NPC = Cast<ANPCCharacterBase>(InteractComponent->GetOwner());
	if (NPC)
	{
		FQuest Quest(NPC->NPCName, QuestTitle);
		for (FQuest CompletableQuest : NPC->CompletableQuests)
		{
			if (CompletableQuest == Quest)
			{
				return CompletableQuest;
			}
		}
	}
	return FQuest();
}

void UInteractWidgetController::AwardQuestRewardsToPlayerState(FString QuestTitle)
{
	FQuest Quest = FindCompletableQuest(QuestTitle);
	if (Quest.QuestTitle.IsEmpty()) return;

	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		for (FQuestReward Reward : Quest.QuestRewards)
		{
			if (Reward.RewardType == EQuestRewardType::XP)
			{
				DonPlayerState->AddToXP(Reward.Amount);
			}
			else
			{
				FItem ItemToAdd = UDonItemLibrary::FindItemByName(GetWorld(), Reward.ItemID);
				DonPlayerState->GetInventoryComponent()->AddItem(ItemToAdd, Reward.Amount);
			}
		}
		DonPlayerState->SetQuestState(Quest, EQuestState::Completed);
		ANPCCharacterBase* NPC = Cast<ANPCCharacterBase>(InteractComponent->GetOwner());
		if (NPC)
		{
			NPC->RemoveCompletedQuest(Quest);
		}
	}
}

FQuest UInteractWidgetController::FindQuestInDialogue()
{
	for (FResponseOption Response : CurrentDialogue.ResponseOptions)
	{
		if (Response.ResponseType == EResponseType::Quest)
		{
			const UDataTable* QuestTable = Response.NextDialogueHandle.DataTable;
			const FName& QuestName = Response.NextDialogueHandle.RowName;
			FQuestRow* QuestRow = QuestTable->FindRow<FQuestRow>(QuestName, "");
			if (QuestRow) return QuestRow->Quest;
		}
	}
	return FQuest(CurrentDialogue.NPCName, CurrentDialogue.Branch);
}

bool UInteractWidgetController::HasQuestInPlayerState(ENPCName NPCName, FString QuestTitle) const
{
	FQuest Quest;
	if (!UDonItemLibrary::FindQuestRow(GetWorld(), Quest, NPCName, QuestTitle)) return false;

	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		return DonPlayerState->HasQuest(Quest);
	}
	return false;
}

void UInteractWidgetController::AddItemToPlayerState(FName ItemName, int32 Amount)
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		FItem ItemToAdd = UDonItemLibrary::FindItemByName(GetWorld(), ItemName);
		DonPlayerState->GetInventoryComponent()->AddItem(ItemToAdd, Amount);
	}
}

bool UInteractWidgetController::HasItemInPlayerState(FObjective& Objective)
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		return DonPlayerState->IsItemConditionMet(FString("Normal"), Objective);
	}
	return false;
}

void UInteractWidgetController::AddDialogueToPlayerState(const FDialogue& Dialogue)
{
	if (!(Dialogue.Type == EDialogueType::ContinueFromLast || Dialogue.Type == EDialogueType::ChoicedResponse)) return;
	
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState)
	{
		DonPlayerState->CompletedDialogues.FindOrAdd(Dialogue.NPCName).Dialogues.AddUnique(Dialogue);
		DonPlayerState->CheckAllQuestObjectives();
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
		DonPlayerState->CheckAllQuestObjectives();
	}
}