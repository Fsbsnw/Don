// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemEquipmentAsset.generated.h"

struct FItemEquipmentInfo;
/**
 * 
 */
UCLASS()
class DON_API UItemEquipmentAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	TArray<FItemEquipmentInfo> EquipmentInformation;
	
	UFUNCTION(BlueprintCallable)
	FItemEquipmentInfo FindItemEquipmentByName(FName ItemName);
};
