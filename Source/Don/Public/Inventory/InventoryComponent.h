// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemAsset.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, FItem, Item);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(APlayerState* PlayerState, FName ItemName);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(APlayerState* PlayerState, FName ItemName);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryItemAdded;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryItemRemoved;
};
