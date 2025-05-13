// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Rampage_RangedState.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "DonGameplayTags.h"

UBTTask_Rampage_RangedState::UBTTask_Rampage_RangedState()
{
	NodeName = TEXT("To Ranged State");
}

EBTNodeResult::Type UBTTask_Rampage_RangedState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIController->GetPawn());
	if (ASC && !ASC->HasMatchingGameplayTag(FDonGameplayTags::Get().Boss_Rampage_Abilities_HoldingRock))
	{
		const FGameplayTag AttackTag = FDonGameplayTags::Get().Boss_Rampage_Abilities_HoldingRock;
		ASC->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
