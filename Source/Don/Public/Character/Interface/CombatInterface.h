// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DON_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool IsDead();

	UFUNCTION(BlueprintNativeEvent)
	void Die(const FVector& DeathImpulse, float ItemDropRate);

	UFUNCTION(BlueprintNativeEvent)
	void ApplyHitEffect();

	UFUNCTION(BlueprintNativeEvent)
	void ResetHitEffect();

	UFUNCTION(BlueprintNativeEvent)
	void SetKnockbackState(bool NewState, const FVector& Force);

	UFUNCTION(BlueprintNativeEvent)
	bool IsItemEquipped(FItem& Item);
	
	UFUNCTION(BlueprintNativeEvent)
	void EquipWeapon(FItem& Item);

	UFUNCTION(BlueprintNativeEvent)
	void EquipArmorHelmet(FItem& Item);

	UFUNCTION(BlueprintNativeEvent)
	void EquipArmorChest(FItem& Item);

	UFUNCTION(BlueprintNativeEvent)
	void EquipArmorLegs(FItem& Item);

	UFUNCTION(BlueprintNativeEvent)
	void EquipArmorHands(FItem& Item);

	UFUNCTION(BlueprintNativeEvent)
	void EquipArmorBoots(FItem& Item);

	UFUNCTION(BlueprintNativeEvent)
	void UnequipAllItems();

	UFUNCTION(BlueprintNativeEvent)
	void UnequipItem(FItem& Item);

	UFUNCTION(BlueprintNativeEvent)
	void UnequipWeapon();
	
	UFUNCTION(BlueprintNativeEvent)
	void UnequipArmorHelmet();

	UFUNCTION(BlueprintNativeEvent)
	void UnequipArmorChest();
	
	UFUNCTION(BlueprintNativeEvent)
	void UnequipArmorHands();
	
	UFUNCTION(BlueprintNativeEvent)
	void UnequipArmorLegs();
	
	UFUNCTION(BlueprintNativeEvent)
	void UnequipArmorBoots();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateUpgradedItemInfo(const FItem& Item);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetRewardScore();
	
	UFUNCTION(BlueprintNativeEvent)
	float GetWeaponDamage();

	UFUNCTION(BlueprintNativeEvent)
	float GetArmorDefense();
	
	UFUNCTION(BlueprintNativeEvent)
	float GetCharacterLevel() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetEquippedArmorCount();
};
