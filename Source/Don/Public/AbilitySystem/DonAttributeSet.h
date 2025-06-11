// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DonAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) 

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	
	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
};

template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class DON_API UDonAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UDonAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bCriticalHit) const;
	void ShowEvadeText(const FEffectProperties& Props, bool bEvade) const;
	
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	
	/*
	 * Vital Attributes
	 */
	

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, MaxHealth);


	/*
	 * Primary Attributes
	 */
	

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, Vigor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Primary Attributes")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, Agility);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Supernatural, Category = "Primary Attributes")
	FGameplayAttributeData Supernatural;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, Supernatural);


	/*
	 * Secondary Attributes
	 */
	

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DodgeChance, Category = "Secondary Attributes")
	FGameplayAttributeData DodgeChance;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, DodgeChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SkillCooldown, Category = "Secondary Attributes")
	FGameplayAttributeData SkillCooldown;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, SkillCooldown);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpawnChance, Category = "Secondary Attributes")
	FGameplayAttributeData SpawnChance;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, SpawnChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ItemDropRate, Category = "Secondary Attributes")
	FGameplayAttributeData ItemDropRate;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, ItemDropRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ExpGainRate, Category = "Secondary Attributes")
	FGameplayAttributeData ExpGainRate;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, ExpGainRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoneyGainRate, Category = "Secondary Attributes")
	FGameplayAttributeData MoneyGainRate;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, MoneyGainRate);

	
	/*
	 * Meta Attributes
	 */
	

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, IncomingXP);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingMoney;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, IncomingMoney);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UDonAttributeSet, IncomingDamage);



	/*
	 * Replication Functions
	 */
	
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldAgility) const;

	UFUNCTION()
	void OnRep_Supernatural(const FGameplayAttributeData& OldSupernatural) const;


	

	UFUNCTION()
	void OnRep_MoneyGainRate(const FGameplayAttributeData& OldMoneyGainRate) const;
	
	UFUNCTION()
	void OnRep_ExpGainRate(const FGameplayAttributeData& OldExpGainRate) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_DodgeChance(const FGameplayAttributeData& OldDodgeChance) const;

	UFUNCTION()
	void OnRep_SkillCooldown(const FGameplayAttributeData& OldSkillCooldown) const;

	UFUNCTION()
	void OnRep_SpawnChance(const FGameplayAttributeData& OldSpawnChance) const;

	UFUNCTION()
	void OnRep_ItemDropRate(const FGameplayAttributeData& OldItemDropRate) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};