// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DonEnemy.h"

#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"

ADonEnemy::ADonEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UDonAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UDonAttributeSet>("AttributeSet");
}

void ADonEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void ADonEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UDonAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
