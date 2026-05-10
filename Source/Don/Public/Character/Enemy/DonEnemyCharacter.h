// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DonCharacterBase.h"
#include "Character/DonCharacterTypes.h"
#include "DonEnemyCharacter.generated.h"

enum class EEnemyClass;
class APatrolPath;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDungeonGroupKilled, int32);

struct FLootableItem;
class UNiagaraSystem;
class UBehaviorTree;
class ADonAIController;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class DON_API ADonEnemyCharacter : public ADonCharacterBase
{
	GENERATED_BODY()
public:
	ADonEnemyCharacter();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Destroyed() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Die_Implementation(const FVector& DeathImpulse, float ItemDropRate) override;

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyStone();
	
	UFUNCTION(BlueprintCallable)
	void SetMeshInitState();

	void SetEnemyLevel(int32 InLevel) { EnemyLevel = InLevel; }
	
	void SetHealthPercent(float NewValue);
	void SetHealthText(float NewValue, float NewMaxValue);
	void SetHealthVisibility(bool State);

	UPROPERTY(EditAnywhere)
	float ForceMultiplier = 15.f;

	UPROPERTY(EditAnywhere)
	float TestXDivide = 2.f;

	UPROPERTY(EditAnywhere)
	float CrystalDropRate = 0.5f;

	UFUNCTION(BlueprintCallable)
	bool IsForwardRagdoll() { return bForwardRagdoll; }

	UPROPERTY(EditDefaultsOnly)
	TArray<FLootableItem> LootableItems;

	UPROPERTY(EditDefaultsOnly)
	EEnemyClass EnemyClass = EEnemyClass::Fighter;

	/*
	 * Boss
	 */

	UFUNCTION(BlueprintCallable)
	void SetIsHoldingRock(bool NewState) { bIsHoldingRock = NewState; }

	UFUNCTION(BlueprintCallable)
	bool GetIsHoldingRock() const { return bIsHoldingRock; }

	UFUNCTION(BlueprintCallable)
	void SetGetupState(bool NewState) { bGetupState = NewState; }

	UFUNCTION(BlueprintCallable)
	bool GetGetupState() const { return bGetupState; }
	
	UPROPERTY(EditDefaultsOnly)
	bool bBossEnemy = false;

	UPROPERTY(EditInstanceOnly)
	APatrolPath* PatrolPath;

	int32 PatrolIndex = 0;

	// Dungeon

	int32 GroupID = -1;
	FOnDungeonGroupKilled OnDungeonGroupKilled;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* HealthBarComponent;

	FTimerHandle HealthVisibilityTimerHandle;
	FTimerHandle KnockbackCollisionTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> DeathEffect;

private:
	int32 EnemyLevel = 1;
	
	UPROPERTY(EditDefaultsOnly)
	FName BodyCenterBone = FName("Hips");

	UPROPERTY(EditDefaultsOnly)
	FName NeckBone = FName("Neck");
	
	bool bForwardRagdoll = false;
	bool bIsHoldingRock = false;
	bool bGetupState = true;
};
