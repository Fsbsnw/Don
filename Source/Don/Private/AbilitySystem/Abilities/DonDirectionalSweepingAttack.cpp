// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DonDirectionalSweepingAttack.h"

#include "Actor/PlayerWeapon.h"
#include "Character/Player/DonCharacter.h"

void UDonDirectionalSweepingAttack::CauseDamage(AActor* TargetActor)
{
	ADonCharacter* DonCharacter = Cast<ADonCharacter>(GetAvatarActorFromActorInfo());
	if (DonCharacter && DonCharacter->GetPlayerWeapon())
	{
		DonCharacter->GetPlayerWeapon()->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		DonCharacter->GetPlayerWeapon()->DamageEffectParams.KnockbackChance = 100.f;
		DonCharacter->GetPlayerWeapon()->DamageEffectParams.KnockbackForceMagnitude = Force * 100.f;
	}
}