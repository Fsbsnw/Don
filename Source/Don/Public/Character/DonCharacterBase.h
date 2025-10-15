// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "Engine/DataTable.h"
#include "GameplayTags.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "DonCharacterBase.generated.h"

struct FItemEquipmentInfo;
struct FDonGameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentChanged);

class ADonEquipmentActor;
struct FItem;

UENUM(BlueprintType)
enum class ECharacterClass
{
	Fighter		UMETA(DisplayName = "Fighter"),
	Warrior		UMETA(DisplayName = "Warrior"),
	Ranger		UMETA(DisplayName = "Ranger")
};

USTRUCT(BlueprintType)
struct FCharacterClassInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterClass CharacterClass = ECharacterClass::Fighter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DroppableMoney = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DroppableXP = 0;
};

class ALootableActor;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class DON_API ADonCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ADonCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Combat Interface
	virtual void Die_Implementation(const FVector& DeathImpulse, float ItemDropRate) override;
	virtual bool IsDead_Implementation() override { return bDead; };
	virtual void ApplyHitEffect_Implementation() override;
	virtual void SetKnockbackState_Implementation(bool NewState, const FVector& Force) override;
	virtual bool IsItemEquipped_Implementation(FItem& Item) override;
	virtual void EquipItem_Implementation(FItem& Item) override;
	virtual void UnequipItem_Implementation(FItem& Item) override;
	virtual void UpdateUpgradedItemInfo_Implementation(const FItem& Item) override;
	virtual int32 GetRewardScore_Implementation() override;
	virtual float GetWeaponDamage_Implementation() override;
	virtual float GetCharacterLevel_Implementation() const override;
	virtual float GetArmorDefense_Implementation() override;
	virtual int32 GetEquippedArmorCount_Implementation() override;
	// End Combat Interface

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TMap<FGameplayTag, TObjectPtr<ADonEquipmentActor>> EquipmentParts;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TMap<FGameplayTag, FName> EquipmentSockets;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultMaxVitalAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	FActiveGameplayEffectHandle PrimaryEffectHandle;
	FActiveGameplayEffectHandle SecondaryEffectHandle;
	FActiveGameplayEffectHandle MaxVitalEffectHandle;
	
	FActiveGameplayEffectHandle ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level);
	void InitializeDefaultAttributes();

	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;

	UPROPERTY(BlueprintReadOnly)
	bool bKnockback = false;

	UPROPERTY(EditDefaultsOnly)
	int32 RewardScore = 1;

	UPROPERTY(EditDefaultsOnly)
	float CharacterLevel = 1.f;
	
	UPROPERTY(EditDefaultsOnly)
	ECharacterClass CharacterClass = ECharacterClass::Fighter;
	
public:
	void SetCharacterLevel(float InLevel) { CharacterLevel = InLevel; }
	void ResetMaterials();
	void AddCharacterAbilities();
	
	UFUNCTION(BlueprintCallable)
	bool GetKnockbackInProgress() const { return bKnockback; }
	
	UFUNCTION(BlueprintCallable)
	void SetKnockback(bool KnockbackState) { bKnockback = KnockbackState; }

	void SpawnAndAttachEquipment(const FGameplayTag& SlotTag, const FItem& Item);
	void DetachAndDestroyEquipment(const FGameplayTag& SlotTag);
	
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentChanged OnEquipmentChanged;

	FORCEINLINE int32 GetBonusAttackPower() const { return BonusAttackPower; } 
	FORCEINLINE int32 GetBonusDefense() const { return BonusDefense; } 
	void AddBonusAttackPower(int32 AttackPower);
	void AddBonusDefense(int32 Defense);

	void UpdateUpgradedArmorPoint(ADonEquipmentActor* Armor, int32 InPoint);
private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupCommonAbilities;

	UPROPERTY(EditAnywhere, Category = "Looting")
	TSubclassOf<ALootableActor> LootableXPClass;
	
	UPROPERTY(EditAnywhere, Category = "Looting")
	TSubclassOf<ALootableActor> LootableMoneyClass;

	UPROPERTY(EditAnywhere, Category = "Looting")
	float TestOffsetLocation = 75.f;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* DeathSound;

	FTimerHandle HitFlashTimerHandle;

	int32 BonusAttackPower = 0;
	int32 BonusDefense = 0;
};