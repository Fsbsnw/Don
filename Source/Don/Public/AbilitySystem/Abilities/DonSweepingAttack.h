// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DonAbilityTypes.h"
#include "AbilitySystem/Abilities/DonDamageGameplayAbility.h"
#include "DonSweepingAttack.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UDonSweepingAttack : public UDonDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void CauseDamage(AActor* TargetActor) override;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(EditAnywhere)
	float Force = 500.f;
	
	UPROPERTY(EditAnywhere)
	float Pitch = 45.f;
};
