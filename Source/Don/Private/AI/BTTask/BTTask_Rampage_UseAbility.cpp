 // Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Rampage_UseAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

 UBTTask_Rampage_UseAbility::UBTTask_Rampage_UseAbility()
{
	NodeName = TEXT("Activate Ability");
}

void UBTTask_Rampage_UseAbility::RotateToTarget(UBehaviorTreeComponent& OwnerComp, AActor* TargetActor)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	FVector Direction = (TargetActor->GetActorLocation() - ControlledPawn->GetActorLocation()).GetSafeNormal();
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	ControlledPawn->SetActorRotation(LookAtRotation);
}

 EBTNodeResult::Type UBTTask_Rampage_UseAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIController->GetPawn());
	if (ASC)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetEnemy")));
		if (!TargetActor) return EBTNodeResult::Failed;

		if (bRotateToTarget) RotateToTarget(OwnerComp, TargetActor);
		
		ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
