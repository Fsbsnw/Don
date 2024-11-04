// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DonAbilitySystemComponent.h"

#include "DonGameplayTags.h"

void UDonAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDonAbilitySystemComponent::EffectApplied);
}

void UDonAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                               const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

FActiveGameplayEffectHandle UDonAbilitySystemComponent::ApplyGameplayEffectSpecToSelf(
	const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey)
{
	if (HasMatchingGameplayTag(FDonGameplayTags::Get().Cheat_Immortal))
	{
		if (GameplayEffect.Def.Get()->HasMatchingGameplayTag(FDonGameplayTags::Get().Damage))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player is Immortal"));
			return FActiveGameplayEffectHandle();
		}
	}
	
	return Super::ApplyGameplayEffectSpecToSelf(GameplayEffect, PredictionKey);
}
