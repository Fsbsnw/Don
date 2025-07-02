// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DonItemBase.generated.h"

struct FItem;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class DON_API UDonItemBase : public UObject
{
	GENERATED_BODY()
public:
	virtual bool UseItem(AActor* Target, FItem& Item, bool& bWasConsumed);
};
