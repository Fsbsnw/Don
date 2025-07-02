// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DonItemBase.h"
#include "DonItem_EquipmentBase.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UDonItem_EquipmentBase : public UDonItemBase
{
	GENERATED_BODY()
public:
	virtual bool UseItem(AActor* Target, FItem& Item, bool& bWasConsumed) override;
};
