// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */

struct FDonGameplayTags
{
public:
	static const FDonGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	// Inputs

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Tab;
	FGameplayTag InputTag_NumKey_1;
	FGameplayTag InputTag_NumKey_2;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_Q;


	// Primary Attributes

	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Agility;
	FGameplayTag Attributes_Primary_Supernatural;


	// Secondary Attributes

	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_DodgeChance;
	FGameplayTag Attributes_Secondary_SkillCooldown;
	FGameplayTag Attributes_Secondary_SpawnChance;
	FGameplayTag Attributes_Secondary_ExpGainRate;
	FGameplayTag Attributes_Secondary_MoneyGainRate;
	FGameplayTag Attributes_Secondary_ItemDropRate;


	// Meta Attributes

	FGameplayTag Attributes_Meta_IncomingXP;
	

	// Damage

	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Fire;


	// Cheat Attributes

	FGameplayTag Cheat_Immortal;


	// Abilities

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_FireBolt;


	// Store State

	FGameplayTag Store_Normal_Open;
	FGameplayTag Store_Normal_Closed;

protected:
 
private:
	static FDonGameplayTags GameplayTags;
};
