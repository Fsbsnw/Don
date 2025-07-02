// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DonCharacterBase.h"
#include "DonEnemy.generated.h"

struct FLootableItem;
class UNiagaraSystem;
class UBehaviorTree;
class ADonAIController;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class DON_API ADonEnemy : public ADonCharacterBase
{
	GENERATED_BODY()
public:
	ADonEnemy();

	virtual void Destroyed() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Die_Implementation(const FVector& DeathImpulse, float ItemDropRate) override;
	virtual void SetKnockbackState_Implementation(bool bKnockback, const FVector& Force) override;

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyStone();
	
	UFUNCTION(BlueprintCallable)
	void SetMeshInitState();
	
	void SetHealthPercent(float NewValue);
	void SetHealthText(float NewValue, float NewMaxValue);
	void SetHealthVisibility(bool State);
	UFUNCTION(BlueprintCallable)
	void SetKnockbackState(bool NewState, FVector Force = FVector::ZeroVector);
	void SetRewardScore(int32 InRewardScore) { RewardScore = InRewardScore; }

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

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* HealthBarComponent;

	FTimerHandle HealthVisibilityTimerHandle;
	FTimerHandle KnockbackCollisionTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ADonAIController> DonAIController;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> DeathEffect;

private:
	bool bForwardRagdoll = false;
	bool bIsHoldingRock = false;
	bool bGetupState = true;
};
