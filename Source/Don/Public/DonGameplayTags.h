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
	FGameplayTag InputTag_SpaceBar;
	FGameplayTag InputTag_Tab;
	FGameplayTag InputTag_ESC;
	FGameplayTag InputTag_NumKey_1;
	FGameplayTag InputTag_NumKey_2;
	FGameplayTag InputTag_NumKey_3;
	FGameplayTag InputTag_NumKey_4;
	FGameplayTag InputTag_NumKey_5;
	FGameplayTag InputTag_NumKey_6;
	FGameplayTag InputTag_E;
	FGameplayTag InputTag_Q;
	FGameplayTag InputTag_J;
	FGameplayTag InputTag_K;


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
	FGameplayTag Attributes_Meta_IncomingMoney;
	

	// Damage

	FGameplayTag Damage;
	FGameplayTag Damage_CriticalHit;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Fire;


	// Effects

	FGameplayTag Effects_HitReact;

	// Cheat Attributes

	FGameplayTag Cheat_Immortal;


	// Abilities
	
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_ComboAttack;
	FGameplayTag Abilities_ComboAttack_1;
	FGameplayTag Abilities_ComboAttack_2;
	FGameplayTag Abilities_ComboAttack_3;
	FGameplayTag Abilities_FireBolt;
	FGameplayTag Abilities_ThrowBoomerangAxe;
	FGameplayTag Abilities_ArcSlash;
	FGameplayTag Abilities_ChronoBreak;
	FGameplayTag Abilities_Lightning;
	FGameplayTag Abilities_Dash;
	FGameplayTag Abilities_DodgeRolling;
	FGameplayTag Request_Abilities_Lightning;
	
	FGameplayTag Cooldown_Attack;
	FGameplayTag Cooldown_FireBolt;

	FGameplayTag Abilities_HitReact;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;


	// Boss Abilities


	FGameplayTag Boss_Rampage_Abilities_HoldingRock;
	FGameplayTag Boss_Rampage_Abilities_NormalAttack;
	FGameplayTag Boss_Rampage_Abilities_ThrowingRock;
	FGameplayTag Boss_Rampage_Abilities_BackDash;
	FGameplayTag Boss_Rampage_Abilities_PushAway;

	FGameplayTag Cooldown_Boss_Rampage_NormalAttack;
	FGameplayTag Cooldown_Boss_Rampage_ThrowingRock;
	FGameplayTag Cooldown_Boss_Rampage_BackDash;
	FGameplayTag Cooldown_Boss_Rampage_PushAway;
	FGameplayTag Cooldown_Boss_Rampage_HoldingRock;
	

	// Store State

	FGameplayTag Store_Normal_Open;
	FGameplayTag Store_Normal_Closed;

	
	// Item

	FGameplayTag Item;
	FGameplayTag Item_Consumable;
	FGameplayTag Item_Quest;
	FGameplayTag Item_Miscellaneous;
	FGameplayTag Item_Equippable;
	FGameplayTag Item_Equippable_Weapon;
	FGameplayTag Item_Equippable_Armor;
	FGameplayTag Item_Equippable_Armor_Helmet;
	FGameplayTag Item_Equippable_Armor_Chest;
	FGameplayTag Item_Equippable_Armor_Hands;
	FGameplayTag Item_Equippable_Armor_Legs;
	FGameplayTag Item_Equippable_Armor_Boots;
	
	
	// Rewards

	FGameplayTag Reward;
	FGameplayTag Reward_XP;
	FGameplayTag Reward_Money;


	TMap<FGameplayTag, FGameplayTag> FindCooldownTags;
	
protected:
 
private:
	static FDonGameplayTags GameplayTags;
};
