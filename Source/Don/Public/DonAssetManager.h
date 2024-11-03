// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "DonAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UDonAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UDonAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
