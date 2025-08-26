// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DonInnLibrary.generated.h"

struct FRoomServiceOrder;
struct FKitchenOrder;
/**
 * 
 */
UCLASS()
class DON_API UDonInnLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// Kitchen
	UFUNCTION(BlueprintPure, Category = "DonInnLibrary | Kitchen")
	static FKitchenOrder FindCuisineByName(const UObject* WorldContextObject, FName CuisineName);

	UFUNCTION(BlueprintCallable, Category = "DonInnLibrary | Kitchen")
	static void AddKitchenOrder(const UObject* WorldContextObject, FKitchenOrder Order);

	UFUNCTION(BlueprintCallable, Category = "DonInnLibrary | Kitchen")
	static FGuid FindNextQueuedKitchenOrder(const UObject* WorldContextObject);


	// RoomService
	UFUNCTION(BlueprintPure, Category = "DonInnLibrary | RoomService")
	static FRoomServiceOrder FindRoomServiceByName(const UObject* WorldContextObject, FName RoomServiceName);

	UFUNCTION(BlueprintCallable, Category = "DonInnLibrary | RoomService")
	static void AddRoomServiceOrder(const UObject* WorldContextObject, FRoomServiceOrder RoomServiceOrder);

	UFUNCTION(BlueprintPure, Category = "DonInnLibrary | RoomService")
	static FRoomServiceOrder RemoveRoomServiceOrder(const UObject* WorldContextObject);
};
