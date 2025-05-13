// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DonDirectionalSweepingAttack.h"

#include "Character/Player/DonCharacter.h"

void UDonDirectionalSweepingAttack::CauseDamage(AActor* TargetActor)
{
	// if (TargetActor) Super::CauseDamage(TargetActor);

	ADonCharacter* DonCharacter = Cast<ADonCharacter>(GetAvatarActorFromActorInfo());
	if (DonCharacter)
	{
		DonCharacter->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		DonCharacter->DamageEffectParams.KnockbackChance = 100.f;
		DonCharacter->DamageEffectParams.KnockbackForceMagnitude = Force * 100.f;
	}
}