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

	GameplayTags.InputTag_Tab = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.Tab"),
	FString("Keyboard Tab")
	);

	GameplayTags.InputTag_NumKey_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.NumKey_1"),
	FString("Number Key 1")
	);

	GameplayTags.InputTag_NumKey_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.NumKey_2"),
	FString("Number Key 2")
	);

	GameplayTags.InputTag_NumKey_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.NumKey_3"),
	FString("Number Key 3")
	);
	
	GameplayTags.InputTag_NumKey_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.NumKey_4"),
	FString("Number Key 4")
	);
	
	GameplayTags.InputTag_NumKey_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.NumKey_5"),
	FString("Number Key 5")
	);
	
	GameplayTags.InputTag_NumKey_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.NumKey_6"),
	FString("Number Key 6")
	);
	
	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.E"),
	FString("Keyboard E")
	);

	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.Q"),
	FString("Keyboard Q")
	);
	
	GameplayTags.InputTag_J = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.J"),
	FString("Keyboard J")
	);

	GameplayTags.InputTag_K = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.K"),
	FString("Keyboard K")
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

	GameplayTags.Damage_CriticalHit = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.CriticalHit"),
	FString("Critical Hit")
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


	// Abilities

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Attack"),
	FString("Attack Ability")
	);
	
	GameplayTags.Abilities_ComboAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.ComboAttack"),
	FString("Combo Attack")
	);
	
	GameplayTags.Abilities_ComboAttack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.ComboAttack.1"),
	FString("ComboAttack 1")
	);

	
	GameplayTags.Abilities_ComboAttack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.ComboAttack.2"),
	FString("ComboAttack 2")
	);
	
	GameplayTags.Abilities_ComboAttack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.ComboAttack.3"),
	FString("ComboAttack 3")
	);
	
	GameplayTags.Abilities_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.FireBolt"),
	FString("FireBolt Ability")
	);

	
	GameplayTags.Abilities_ThrowBoomerangAxe = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.ThrowBoomerangAxe"),
	FString("ThrowBoomerangAxe Ability")
	);

	
	GameplayTags.Abilities_ArcSlash = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.ArcSlash"),
	FString("ArcSlash Ability")
	);

	
	GameplayTags.Abilities_ChronoBreak = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.ChronoBreak"),
	FString("ChronoBreak Ability")
	);

	GameplayTags.Abilities_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Lightning"),
	FString("Lightning Ability")
	);

	GameplayTags.Abilities_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Dash"),
	FString("Dash Ability")
	);

	GameplayTags.Request_Abilities_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Request.Abilities.Lightning"),
	FString("Request Lightning Ability")
	);

	
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.HitReact"),
	FString("HitReact")
	);

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Status.Locked"),
	FString("Locked Ability")
	);

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Status.Unlocked"),
	FString("Unlocked Ability")
	);

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Status.Equipped"),
	FString("Equipped Ability")
	);

	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Type.Offensive"),
	FString("Offensive Ability")
	);

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Type.Passive"),
	FString("Passive Ability")
	);

	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Type.None"),
	FString("None Type Ability")
	);


	// Cooldown

	GameplayTags.Cooldown_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Cooldown.Attack"),
	FString("Attack Cooldown")
	);

	GameplayTags.Cooldown_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Cooldown.FireBolt"),
	FString("FireBolt Cooldown")
	);
	

	// Meta Attributes

	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Meta.IncomingXP"),
	FString("IncomingXP Meta Attribute")
	);

	GameplayTags.Attributes_Meta_IncomingMoney = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Meta.IncomingMoney"),
	FString("IncomingMoney Meta Attribute")
	);
	
	// Store State

	GameplayTags.Store_Normal_Open = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Store.Normal.Open"),
	FString("Normal Store Open")
	);

	GameplayTags.Store_Normal_Closed = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Store.Normal.Closed"),
	FString("Normal Store Closed")
	);

	
	// Item

	GameplayTags.Item = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item"),
	FString("Item")
	);

	GameplayTags.Item_Consumable = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Consumable"),
	FString("Consumable Item")
	);
	
	GameplayTags.Item_Miscellaneous = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Miscellaneous"),
	FString("Miscellaneous Item")
	);

	GameplayTags.Item_Quest = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Quest"),
	FString("Quest Item")
	);

	GameplayTags.Item_Equippable = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Equippable"),
	FString("Equippable Item")
	);

	GameplayTags.Item_Equippable_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Equippable.Weapon"),
	FString("Equippable Weapon")
	);

	GameplayTags.Item_Equippable_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Equippable.Armor"),
	FString("Equippable Armor")
	);

	GameplayTags.Item_Equippable_Armor_Helmet = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Equippable.Armor.Helmet"),
	FString("Equippable Armor Helmet")
	);

	GameplayTags.Item_Equippable_Armor_Chest = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Equippable.Armor.Chest"),
	FString("Equippable Armor Chest")
	);

	GameplayTags.Item_Equippable_Armor_Hands = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Equippable.Armor.Hands"),
	FString("Equippable Armor Hands")
	);

	GameplayTags.Item_Equippable_Armor_Legs = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Equippable.Armor.Legs"),
	FString("Equippable Armor Legs")
	);

	GameplayTags.Item_Equippable_Armor_Boots = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Item.Equippable.Armor.Boots"),
	FString("Equippable Armor Boots")
	);


	// Rewards

	GameplayTags.Reward = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Reward"),
	FString("Reward")
	);

	GameplayTags.Reward_XP = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Reward.XP"),
	FString("Reward XP")
	);

	GameplayTags.Reward_Money = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Reward.Money"),
	FString("Reward Money")
	);
}
