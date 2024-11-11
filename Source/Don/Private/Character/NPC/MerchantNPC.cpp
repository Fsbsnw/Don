// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/MerchantNPC.h"

#include "Character/Component/InteractComponent.h"

void AMerchantNPC::Interact(APlayerState* TargetPlayerState)
{
	GetInteractComponent()->OpenStore(TargetPlayerState);
}
