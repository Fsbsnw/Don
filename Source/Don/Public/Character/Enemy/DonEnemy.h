// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DonCharacterBase.h"
#include "DonEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DON_API ADonEnemy : public ADonCharacterBase
{
	GENERATED_BODY()
public:
	ADonEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
