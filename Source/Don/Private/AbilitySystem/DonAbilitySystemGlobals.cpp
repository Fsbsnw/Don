// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DonAbilitySystemGlobals.h"

#include "DonAbilityTypes.h"

FGameplayEffectContext* UDonAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FDonGameplayEffectContext();
}
