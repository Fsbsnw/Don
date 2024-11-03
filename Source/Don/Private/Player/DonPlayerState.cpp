// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DonPlayerState.h"

#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"
#include "Inventory/InventoryComponent.h"

ADonPlayerState::ADonPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDonAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UDonAttributeSet>("AttributeSet");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory Component");
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* ADonPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
