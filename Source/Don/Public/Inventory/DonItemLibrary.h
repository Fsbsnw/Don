// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/ItemAsset.h"
#include "DonItemLibrary.generated.h"

class UInventoryWidgetController;
/**
 * 
 */
UCLASS()
class DON_API UDonItemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static FItem FindItemByName(const UObject* WorldContextObject, FName ItemName);

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static UInventoryWidgetController* GetInventoryWidgetController(const UObject* WorldContextObject);
};
