// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemAsset.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DON_API IInventoryInterface
{
	GENERATED_BODY()

public:
	virtual const FItem GetItemInfo() const = 0;
};
