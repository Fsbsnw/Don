// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "Interface/CombatInterface.h"
#include "DonPawnBase.generated.h"

class UFloatingPawnMovement;
class UCapsuleComponent;
struct FItemEquipmentInfo;
struct FDonGameplayTags;

class ADonEquipmentActor;
struct FItem;

class ALootableActor;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class DON_API ADonPawnBase : public APawn, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ADonPawnBase();

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	
	// GAS
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Combat Interface
	virtual void Die_Implementation(const FVector& DeathImpulse, float ItemDropRate) override;
	virtual bool IsDead_Implementation() override { return bDead; };
	virtual void ApplyHitEffect_Implementation() override;
	virtual void SetKnockbackState_Implementation(bool NewState, const FVector& Force) override;
	virtual float GetCharacterLevel_Implementation() const override;
	// End Combat Interface

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UFloatingPawnMovement* MovementComponent;
	
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
	
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
	
	void ResetMaterials();
	void AddCharacterAbilities();
	void InitializeDefaultAttributes();
	FActiveGameplayEffectHandle ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level);
	
public:
	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DeathSound;

	FTimerHandle HitFlashTimerHandle;

private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupCommonAbilities;
};