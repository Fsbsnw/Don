// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DonCharacterBase.generated.h"

UCLASS(Abstract)
class DON_API ADonCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADonCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
public:
};
