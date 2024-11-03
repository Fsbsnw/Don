// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DonAbilitySystemComponent.h"

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
