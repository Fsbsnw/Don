// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "DonCharacterBase.generated.h"


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
	virtual void Die_Implementation(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() override { return bDead; };
	virtual void ApplyHitEffect_Implementation() override;
	virtual void SetKnockbackState_Implementation(bool NewState, const FVector& Force) override;
	virtual bool IsItemEquipped_Implementation(FItem& Item) override;
	virtual void UnequipAllItems_Implementation() override;
	virtual void UnequipItem_Implementation(FItem& Item) override;
	virtual void UnequipWeapon_Implementation() override;
	virtual void UnequipArmorHelmet_Implementation() override;
	virtual void UnequipArmorChest_Implementation() override;
	virtual void UnequipArmorHands_Implementation() override;
	virtual void UnequipArmorLegs_Implementation() override;
	virtual void UnequipArmorBoots_Implementation() override;
	virtual void EquipWeapon_Implementation(FItem& Item) override;
	virtual void EquipArmorHelmet_Implementation(FItem& Item) override;
	virtual void EquipArmorChest_Implementation(FItem& Item) override;
	virtual void EquipArmorHands_Implementation(FItem& Item) override;
	virtual void EquipArmorLegs_Implementation(FItem& Item) override;
	virtual void EquipArmorBoots_Implementation(FItem& Item) override;
	virtual float GetWeaponDamage_Implementation() override;
	// End Combat Interface

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<ADonEquipmentActor> Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	TObjectPtr<ADonEquipmentActor> ArmorHelmet;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	TObjectPtr<ADonEquipmentActor> ArmorChest;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	TObjectPtr<ADonEquipmentActor> ArmorLeftHand;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	TObjectPtr<ADonEquipmentActor> ArmorRightHand;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	TObjectPtr<ADonEquipmentActor> ArmorLegs;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	TObjectPtr<ADonEquipmentActor> ArmorLeftBoot;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	TObjectPtr<ADonEquipmentActor> ArmorRightBoot;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	FName ArmorHelmetSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	FName ArmorChestSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	FName ArmorLeftHandSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	FName ArmorRightHandSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	FName ArmorLegsSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	FName ArmorLeftBootSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	FName ArmorRightBootSocket;
	
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
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	void InitializeDefaultAttributes() const;

	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;

	UPROPERTY(BlueprintReadOnly)
	bool bKnockback = false;
	
	UPROPERTY(EditDefaultsOnly)
	ECharacterClass CharacterClass = ECharacterClass::Fighter;
public:
	void ResetMaterials();
	void AddCharacterAbilities();
	UFUNCTION(BlueprintCallable)
	bool GetKnockbackInProgress() const { return bKnockback; };
	UFUNCTION(BlueprintCallable)
	void SetKnockback(bool KnockbackState) { bKnockback = KnockbackState; };
private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Looting")
	TSubclassOf<ALootableActor> LootableXPClass;
	
	UPROPERTY(EditAnywhere, Category = "Looting")
	TSubclassOf<ALootableActor> LootableMoneyClass;

	UPROPERTY(EditAnywhere, Category = "Looting")
	float TestOffsetLocation = 75.f;

	FTimerHandle HitFlashTimerHandle;
};