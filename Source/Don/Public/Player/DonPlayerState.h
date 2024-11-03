// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Data/ItemAsset.h"
#include "GameFramework/PlayerState.h"
#include "DonPlayerState.generated.h"

class UInventoryComponent;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class DON_API ADonPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ADonPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItem>& GetInventory() { return Inventory; }
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY()
	TArray<FItem> Inventory;
public:
	const uint8 MaxItemSlots = 20; 
};
