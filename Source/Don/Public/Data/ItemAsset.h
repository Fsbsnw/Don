// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemAsset.generated.h"


struct FItem;

/**
 * 
 */
UCLASS()
class DON_API UItemAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	TArray<FItem> ItemInformation;
	
	UFUNCTION(BlueprintCallable)
	FItem FindItemByName(FName ItemName);
};
