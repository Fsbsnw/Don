// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/NormalNPC.h"

#include "Character/Component/InteractComponent.h"

void ANormalNPC::Interact(APlayerState* TargetPlayerState)
{
	GetInteractComponent()->OpenDialogue(TargetPlayerState);
}
