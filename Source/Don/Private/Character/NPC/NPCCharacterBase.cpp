// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/NPCCharacterBase.h"

#include "Character/Component/InteractComponent.h"

ANPCCharacterBase::ANPCCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractComponent = CreateDefaultSubobject<UInteractComponent>("Interact Component");

}

UInteractComponent* ANPCCharacterBase::GetInteractComponent()
{
	return InteractComponent;
}

void ANPCCharacterBase::Interact(APlayerState* TargetPlayerState)
{
}