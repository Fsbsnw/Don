// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NPC/NPCCharacterBase.h"
#include "MerchantNPC.generated.h"

/**
 * 
 */
UCLASS()
class DON_API AMerchantNPC : public ANPCCharacterBase
{
	GENERATED_BODY()
public:
	virtual void Interact(APlayerState* TargetPlayerState) override;
};
