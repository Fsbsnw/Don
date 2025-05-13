// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Rampage_RandomAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "DonGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Rampage_RandomAbility::UBTTask_Rampage_RandomAbility()
{
	NodeName = TEXT("Randomly Activate Ability");
}

void UBTTask_Rampage_RandomAbility::RotateToTarget(UBehaviorTreeComponent& OwnerComp, AActor* TargetActor)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	FVector Direction = (TargetActor->GetActorLocation() - ControlledPawn->GetActorLocation()).GetSafeNormal();
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	ControlledPawn->SetActorRotation(LookAtRotation);
}

EBTNodeResult::Type UBTTask_Rampage_RandomAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (AbilityTags.IsEmpty()) return EBTNodeResult::Failed;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerComp.GetAIOwner()->GetPawn());
	if (ASC)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetEnemy")));
		if (!TargetActor) return EBTNodeResult::Failed;

		if (bRotateToTarget) RotateToTarget(OwnerComp, TargetActor);

		const FGameplayTag AbilityTag = AbilityTags[FMath::RandRange(0, AbilityTags.Num() - 1)];
		const FGameplayTag* CooldownTag = FDonGameplayTags::Get().FindCooldownTags.Find(AbilityTag);

		if (ASC->HasMatchingGameplayTag(*CooldownTag)) return EBTNodeResult::Failed;
		
		ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
