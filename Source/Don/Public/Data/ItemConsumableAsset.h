// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemConsumableAsset.generated.h"

struct FItemConsumableInfo;
/**
 * 
 */
UCLASS()
class DON_API UItemConsumableAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	TArray<FItemConsumableInfo> ConsumableInformation;
	
	UFUNCTION(BlueprintCallable)
	FItemConsumableInfo FindItemConsumableByName(FName ItemName);
};
