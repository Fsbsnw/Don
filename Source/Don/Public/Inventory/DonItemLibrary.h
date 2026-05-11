// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DonEquipmentActor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/ItemStructs.h"
#include "DonItemLibrary.generated.h"

class UDonItemBase;
struct FGameplayTag;
class UInventoryWidgetController;
/**
 * 
 */
UCLASS()
class DON_API UDonItemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static UDonItemBase* CreateItemObjectByTag(const UObject* WorldContextObject, FGameplayTag Tag);
	
	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static FItem FindItemByName(const UObject* WorldContextObject, FName ItemName);

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static FItemEquipmentInfo FindItemEquipmentByName(const UObject* WorldContextObject, FName ItemName);
	
	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static FItemConsumableInfo FindItemConsumableByName(const UObject* WorldContextObject, FName ItemName);
	
	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Equipment")
	static FEquipmentAttributes FindEquipmentAttributes(const UObject* WorldContextObject, FName EquipmentName);

	UFUNCTION(BlueprintCallable, Category = "DonItemLibrary | Item")
	static void SpawnLootableXP(const UObject* WorldContextObject, int32 Amount, FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION(BlueprintCallable, Category = "DonItemLibrary | Item")
	static void SpawnLootableMoney(const UObject* WorldContextObject, int32 Amount, int32 Quantity, FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION(BlueprintCallable, Category = "DonItemLibrary | Item")
	static void SpawnLootableItem(const UObject* WorldContextObject, TArray<FLootableItem> Items, FVector SpawnLocation, FRotator SpawnRotation, float ItemDropRate);
};