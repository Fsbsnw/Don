// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SetNextPatrolPoint.h"

#include "AIController.h"
#include "Actor/PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/DonEnemyCharacter.h"

EBTNodeResult::Type UBTTask_SetNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	ADonEnemyCharacter* Enemy = Cast<ADonEnemyCharacter>(AIPawn);
	if (!Enemy || !Enemy->PatrolPath) return EBTNodeResult::Failed;

	const TArray<AActor*>& PatrolPoints = Enemy->PatrolPath->PatrolPoints;
	int32 PathLength = PatrolPoints.Num();

	if (PathLength == 0) return EBTNodeResult::Failed;

	AActor* TargetPoint = PatrolPoints[Enemy->PatrolIndex];
	FVector TargetLocation = TargetPoint->GetActorLocation();

	// 다음 인덱스
	Enemy->PatrolIndex = (Enemy->PatrolIndex + 1) % PathLength;

	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		Blackboard->SetValueAsVector(TEXT("TargetLocation"), TargetLocation);
	}

	return EBTNodeResult::Succeeded;
}
