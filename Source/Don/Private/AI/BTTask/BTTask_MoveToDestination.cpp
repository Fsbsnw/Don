// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_MoveToDestination.h"

#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_MoveToDestination::UBTTask_MoveToDestination()
{
	NodeName = "Move To Destination";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		FVector TargetLocation = Customer->SetDestination(Destination);
		OwnerComp.GetAIOwner()->MoveToLocation(TargetLocation);
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_MoveToDestination::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		// 이동 상태를 체크 (Request가 완료되었거나, 경로가 없거나, 도착했는지)
		EPathFollowingStatus::Type Status = AIController->GetMoveStatus();

		if (Status == EPathFollowingStatus::Idle)
		{
			// 이동이 끝났다면 (목적지 도착) 태스크 성공 종료!
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}