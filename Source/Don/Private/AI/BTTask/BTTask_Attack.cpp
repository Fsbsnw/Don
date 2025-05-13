// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Attack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "DonGameplayTags.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIController->GetPawn());
	if (ASC)
	{
		const FGameplayTag AttackTag = FDonGameplayTags::Get().Abilities_Attack;
		ASC->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
