// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DonItem_EquipmentBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/Interface/CombatInterface.h"
#include "Data/ItemStructs.h"

bool UDonItem_EquipmentBase::UseItem(AActor* Target, FItem& Item, bool& bWasConsumed)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		AActor* Avatar = TargetASC->GetAvatarActor();
		if (IsValid(Avatar) && Avatar->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_EquipItem(Avatar, Item);
			return true;
		}
	}
	return Super::UseItem(Target, Item, bWasConsumed);
}