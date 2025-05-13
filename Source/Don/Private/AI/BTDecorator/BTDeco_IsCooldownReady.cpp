// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDeco_IsCooldownReady.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"

UBTDeco_IsCooldownReady::UBTDeco_IsCooldownReady()
{
	NodeName = TEXT("Is Cooldown Ready");
}

bool UBTDeco_IsCooldownReady::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return false;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn);
	if (!ASC) return false;

	return !ASC->HasMatchingGameplayTag(CooldownTag);
}
