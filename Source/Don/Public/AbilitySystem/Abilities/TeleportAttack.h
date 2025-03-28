// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DonDamageGameplayAbility.h"
#include "TeleportAttack.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UTeleportAttack : public UDonDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDelayMultiplier() { return DelayMultiplier; };
	
	UFUNCTION(BlueprintCallable)
	bool FindCombatTarget();

	UFUNCTION(BlueprintImplementableEvent)
	void TeleportToTarget();

	UFUNCTION(BlueprintImplementableEvent)
	void RequestEndAbility();

	UPROPERTY(EditAnywhere)
	float TeleportSpeed = 5.f;

	UPROPERTY(EditAnywhere)
	float CombatRadius = 400.f;

	UPROPERTY(EditAnywhere)
	float OverDistance = 200.f;

private:
	void SetTeleportTarget();
	UFUNCTION()
	void SetTransformToTarget(float Value);

	int32 TargetIndex = 0;
	float SpeedMultiplier = 1.f;
	float DelayMultiplier = 1.f;
	
	FVector StartLocation;
	FVector TargetLocation;
	FVector FinalLocation;
	
	UPROPERTY()
	TArray<AActor*> OverlappedActors;
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
}; 
