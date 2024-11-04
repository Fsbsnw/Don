// Fill out your copyright notice in the Description page of Project Settings.


#include "DonGameplayTags.h"
#include "GameplayTagsManager.h"

FDonGameplayTags FDonGameplayTags::GameplayTags;

void FDonGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Inputs
	 */
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.LMB"),
	FString("Left Mouse Button")
	);
	
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.RMB"),
	FString("Right Mouse Button")
	);

	GameplayTags.InputTag_Inventory = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.Inventory"),
	FString("Open / Close Inventory")
	);
		
	
	/*
	 * Primary Attributes 
	 */
	
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health")
	);

	GameplayTags.Attributes_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Agility"),
	FString("Increases Agility")
	);

	GameplayTags.Attributes_Primary_Supernatural = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Supernatural"),
	FString("Increases Supernatural power")
	);
	

	/*
	 * Secondary Attributes 
	 */
	
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.CriticalHitChance"),
	FString("Increases Critical Hit Chance")
	);

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.CriticalHitDamage"),
	FString("Increases Critical Hit Damage")
	);

	GameplayTags.Attributes_Secondary_DodgeChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.DodgeChance"),
	FString("Increases Dodge Chance")
	);

	GameplayTags.Attributes_Secondary_SkillCooldown = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.SkillCooldown"),
	FString("Decreases Skill Cooldown")
	);

	GameplayTags.Attributes_Secondary_SpawnChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.SpawnChance"),
	FString("Increases Enemy Spawn Chance")
	);

	GameplayTags.Attributes_Secondary_ExpGainRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.ExpGainRate"),
	FString("Increases the Rate of Experience Point Gain")
	);

	GameplayTags.Attributes_Secondary_MoneyGainRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.MoneyGainRate"),
	FString("Increases the Rate of Money Gain")
	);

	GameplayTags.Attributes_Secondary_ItemDropRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.ItemDropRate"),
	FString("Increases the Rate of Item Drop")
	);


	// Damage

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage"),
	FString("Normal Damage")
	);

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Physical"),
	FString("Physical Damage")
	);

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Fire"),
	FString("Fire Damage")
	);

	
	// Cheat Attributes

	GameplayTags.Cheat_Immortal = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Cheat.Immortal"),
	FString("Immortal Cheat")
	);
}
