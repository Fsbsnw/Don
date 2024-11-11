// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InteractWidgetController.h"

#include "Character/Component/InteractComponent.h"
#include "Character/NPC/NPCCharacterBase.h"
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
	
	FDialogueContainer* DialoguesForNPC = CompletedDialogues.Find(NPCCharacter->CharacterName);
	FDialogue InitialDialogue;

	if (DialoguesForNPC && DialoguesForNPC->Dialogues.Num() > 0)
	{
		int32 Progress = 0;
		
		for (FDialogue Dialogue : DialoguesForNPC->Dialogues)
		{
			if (Dialogue.DialogueProgress > Progress)
			{
				Progress = Dialogue.DialogueProgress;
				InitialDialogue = Dialogue;
			}
		}
		InitialDialogue = UDonItemLibrary::FindDialogueRow(GetWorld(), InitialDialogue.DialogueID, InitialDialogue.NextDialogueProgress);
	}
	else
	{
		InitialDialogue = UDonItemLibrary::FindDialogueRow(GetWorld(), NPCCharacter->CharacterName, 1);
	}
	
	return InitialDialogue;
}
