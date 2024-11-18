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

FDialogue UInteractWidgetController::SetCurrentDialogueProgress()
{
	TMap<FString, FDialogueContainer> CompletedDialogues = Cast<ADonPlayerState>(PlayerState)->CompletedDialogues;
	ANPCCharacterBase* NPCCharacter = Cast<ANPCCharacterBase>(InteractComponent->GetOwner());
	
	FDialogueContainer* EndedDialoguesForNPC = CompletedDialogues.Find(NPCCharacter->CharacterName);
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
		UDonItemLibrary::FindDialogueRow(GetWorld(), InitialDialogue, NPCCharacter->CharacterName, 1);
	}
	
	return InitialDialogue;
}

FDialogue UInteractWidgetController::FindDialogueRow(const FString& DialogueID, const int32 Progress)
{
	FDialogue Dialogue;
	UDonItemLibrary::FindDialogueRow(GetWorld(), Dialogue, DialogueID, Progress);
	return Dialogue;
}