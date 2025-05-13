// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Rampage_ThrowingRock.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "DonGameplayTags.h"

UBTTask_Rampage_ThrowingRock::UBTTask_Rampage_ThrowingRock()
{
	NodeName = TEXT("Rampage Throwing Rock");
}

EBTNodeResult::Type UBTTask_Rampage_ThrowingRock::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIController->GetPawn());
	if (ASC)
	{
		const FGameplayTag AttackTag = FDonGameplayTags::Get().Boss_Rampage_Abilities_ThrowingRock;
		ASC->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
