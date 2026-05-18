// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DonCharacterTypes.h"
#include "Character/DonPawnBase.h"
#include "GameFramework/Pawn.h"
#include "DonEnemyPawn.generated.h"


class UNiagaraSystem;
class UWidgetComponent;
class UBehaviorTree;

UCLASS()
class DON_API ADonEnemyPawn : public ADonPawnBase
{
	GENERATED_BODY()

public:
	ADonEnemyPawn();

	virtual void InitAbilityActorInfo() override;

protected:
	virtual void Destroyed() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Die_Implementation(const FVector& DeathImpulse, float ItemDropRate) override;
	virtual void SetKnockbackState_Implementation(bool NewState, const FVector& Force) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* HealthBarComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> DeathEffect;

	// Looting
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FItemLootableInfo> LootableItems;

	UPROPERTY(EditAnywhere, Category = "Looting")
	TSubclassOf<ALootableActor> LootableXPClass;
	
	UPROPERTY(EditAnywhere, Category = "Looting")
	TSubclassOf<ALootableActor> LootableMoneyClass;
	
	UPROPERTY(EditAnywhere, Category = "Looting")
	float LootOffsetLocation = 75.f;

public:
	UPROPERTY(EditDefaultsOnly)
	float CharacterLevel = 1.f;
	
	UPROPERTY(EditDefaultsOnly)
	EEnemyClass EnemyClass = EEnemyClass::Fighter;

	UPROPERTY(EditAnywhere)
	float ForceMultiplier = 15.f;

	UPROPERTY(EditAnywhere)
	float TestXDivide = 2.f;

	UPROPERTY(EditDefaultsOnly)
	FName BodyCenterBone = FName("Hips");
	
	UPROPERTY(EditDefaultsOnly)
	FName NeckBone = FName("Neck");

	FTimerHandle KnockbackCollisionTimerHandle;

	UFUNCTION(BlueprintCallable)
	bool GetKnockbackInProgress() const { return bKnockback; }
	UFUNCTION(BlueprintCallable)
	bool GetGetupState() const { return bGetupState; }
	UFUNCTION(BlueprintCallable)
	bool IsForwardRagdoll() { return bForwardRagdoll; }
	
	UFUNCTION(BlueprintCallable)
	void SetKnockback(bool KnockbackState) { bKnockback = KnockbackState; }
	UFUNCTION(BlueprintCallable)
	void SetGetupState(bool NewState) { bGetupState = NewState; }

private:
	bool bKnockback = false;
	bool bForwardRagdoll = false;
	bool bGetupState = true;

	FTimerHandle HealthVisibilityTimerHandle;

	void SetHealthVisibility(bool NewState);
	void SetHealthText(float NewHealth, float NewMaxHealth);
	void SetHealthPercent(float NewHealth);

	FVector CurrentMoveDirection = FVector::ZeroVector; 
	FVector CurrentTargetLocation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere)
	float ResetDistanceThreshold = 200.f;

// LOD 설정
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