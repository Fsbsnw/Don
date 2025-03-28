// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DonCharacterBase.h"
#include "DonEnemy.generated.h"

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

	virtual void SetKnockbackState_Implementation(bool bKnockback, const FVector& Force) override;
	UFUNCTION(BlueprintCallable)
	void SetMeshInitState();

	virtual void PossessedBy(AController* NewController) override;
	virtual void Die_Implementation(const FVector& DeathImpulse) override;
	
	void SetHealthPercent(float NewValue);
	void SetHealthText(float NewValue, float NewMaxValue);
	UFUNCTION(BlueprintCallable)
	void SetKnockbackState(bool NewState, FVector Force = FVector::ZeroVector);

	UFUNCTION(BlueprintImplementableEvent)
	void TestAnimation();

	UPROPERTY(EditAnywhere)
	float ForceMultiplier = 15.f;

	UPROPERTY(EditAnywhere)
	float TestXDivide = 2.f;

	UFUNCTION(BlueprintCallable)
	bool IsForwardRagdoll() { return bForwardRagdoll; }

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* HealthBarComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ADonAIController> DonAIController;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> DeathEffect;

private:
	bool bForwardRagdoll = false;
};
