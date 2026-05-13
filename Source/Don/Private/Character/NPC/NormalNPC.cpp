// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/NormalNPC.h"

#include "DonGameplayTags.h"

FInteractionWidgetContext ANormalNPC::Interact_Implementation()
{
	FInteractionWidgetContext IWC;
	IWC.InteractionType = EInteractionType::StartDialogue;
	IWC.InteractionTarget = this;
	IWC.WidgetTag = FDonGameplayTags::Get().UI_Interact_Dialog;

	return IWC;
}
