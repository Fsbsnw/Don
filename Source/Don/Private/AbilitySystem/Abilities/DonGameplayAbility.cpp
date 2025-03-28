// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DonGameplayAbility.h"

FString UDonGameplayAbility::GetDescription(int32 Level)
{
	return FString();
}

FString UDonGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString();
}

float UDonGameplayAbility::GetManaCost(float InLevel) const
{
	return 0.f;
}

float UDonGameplayAbility::GetCooldown(float InLevel) const
{
	return 0.f;
}
