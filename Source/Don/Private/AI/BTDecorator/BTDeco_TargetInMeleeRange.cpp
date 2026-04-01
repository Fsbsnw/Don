// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDeco_TargetInMeleeRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDeco_TargetInMeleeRange::UBTDeco_TargetInMeleeRange()
{
	NodeName = TEXT("Target In Melee Range");
}

bool UBTDeco_TargetInMeleeRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const APawn* AIPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (AIPawn == nullptr) return false;
	
	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		if (AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject("TargetPlayer")))
		{
			const float DistanceToPlayer = FVector::Dist(AIPawn->GetActorLocation(), Player->GetActorLocation());
			return DistanceToPlayer <= Distance;
		}
	}
	return false;
}
