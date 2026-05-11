// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DonGameStateBase.generated.h"

class UUIConfigDataAsset;
/**
 * 
 */
UCLASS()
class DON_API ADonGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	UUIConfigDataAsset* UIConfigDataAsset;
};
