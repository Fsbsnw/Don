// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DonItem_PotionBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Data/ItemStructs.h"
#include "Inventory/DonItemLibrary.h"

bool UDonItem_PotionBase::UseItem(AActor* Target, FItem& Item, bool& bWasConsumed)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
		TSubclassOf<UGameplayEffect> GameplayEffect = UDonItemLibrary::FindItemConsumableByName(this, Item.ItemName).ItemEffectClass;
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffect, 1.0f, ContextHandle);
		TargetASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
		bWasConsumed = true;
		return true;
	}
	return Super::UseItem(Target, Item, bWasConsumed);
}
