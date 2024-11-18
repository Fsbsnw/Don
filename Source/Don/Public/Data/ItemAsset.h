// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemAsset.generated.h"

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	
	UPROPERTY()
	FGuid ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Durability = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Upgrade = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventorySlotIndex = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;
};


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
