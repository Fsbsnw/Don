// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DonGameModeBase.generated.h"

class UItemAsset;

/**
 * 
 */
UCLASS()
class DON_API ADonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Information")
	TObjectPtr<UItemAsset> ItemInfoAsset;
};
