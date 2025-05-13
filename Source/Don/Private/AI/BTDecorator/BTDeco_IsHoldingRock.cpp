// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDeco_IsHoldingRock.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "DonGameplayTags.h"

UBTDeco_IsHoldingRock::UBTDeco_IsHoldingRock()
{
	NodeName = TEXT("Is Holding Rock");
}

bool UBTDeco_IsHoldingRock::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn);
	if (ASC) return ASC->HasMatchingGameplayTag(FDonGameplayTags::Get().Boss_Rampage_Abilities_HoldingRock);
	
	return false;
}
