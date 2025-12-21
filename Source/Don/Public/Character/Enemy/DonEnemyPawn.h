// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "Character/DonCharacterBase.h"
#include "Character/Interface/CombatInterface.h"
#include "GameFramework/Pawn.h"
#include "DonEnemyPawn.generated.h"

struct FLootableItem;
class UNiagaraSystem;
class UBehaviorTree;
class UWidgetComponent;
class UAttributeSet;
class UFloatingPawnMovement;

UCLASS()
class DON_API ADonEnemyPawn : public APawn, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ADonEnemyPawn();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void ApplyHitEffect_Implementation() override;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void InitAbilityActorInfo();
	virtual void PossessedBy(AController* NewController) override;
	virtual void Die_Implementation(const FVector& DeathImpulse, float ItemDropRate) override;
	void InitializeDefaultAttributes();
	void AddCharacterAbilities();
	void ResetMaterials();
	FActiveGameplayEffectHandle ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level);
	void SetCharacterLevel(float InLevel) { CharacterLevel = InLevel; }

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> DeathEffect;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FLootableItem> LootableItems;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UFloatingPawnMovement* MovementComponent;
	
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

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* HealthBarComponent;
	
	UPROPERTY(EditDefaultsOnly)
	float CharacterLevel = 1.f;
	
	UPROPERTY(EditDefaultsOnly)
	ECharacterClass CharacterClass = ECharacterClass::Fighter;
	
	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;

	UPROPERTY(BlueprintReadWrite)
	bool bKnockback = false;

private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupCommonAbilities;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* DeathSound;

	FTimerHandle HitFlashTimerHandle;

	FTimerHandle HealthVisibilityTimerHandle;

	void SetHealthVisibility(bool NewState);
	void SetHealthText(float NewHealth, float NewMaxHealth);
	void SetHealthPercent(float NewHealth);

	FVector CurrentMoveDirection = FVector::ZeroVector; 
	FVector CurrentTargetLocation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere)
	float ResetDistanceThreshold = 200.f;

public:
	void CheckAndUpdateAILOD(const FVector& PlayerLocation);

	UPROPERTY(EditAnywhere)
	float RandomRadius = 300.f;
	
	UPROPERTY(EditAnywhere)
	float AcceptableRadius = 50.f;

	UPROPERTY(EditAnywhere)
	int32 CurrentAILODLevel = 0;
	
	UPROPERTY(EditAnywhere)
	float AILODThreshold = 500.f;

	UPROPERTY(EditAnywhere)
	bool TestBTCondition = true;
};