// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_MoveToLocationAndRepath.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_MoveToLocationAndRepath::UBTTask_MoveToLocationAndRepath()
{
	NodeName = "Move To Location And Repath";

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToLocationAndRepath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 목표 위치 저장
	const FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	// 이동 
	AIController->MoveToLocation(TargetLocation, AcceptableRadius);
	
	// 이동 시작 및 타이머 초기화
	Timer = 0.0f;
	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToLocationAndRepath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UPathFollowingComponent* PathFollowingComp = AIController->GetPathFollowingComponent();
	if (PathFollowingComp == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 1. 이동 중인 경우
	if (PathFollowingComp->GetStatus() == EPathFollowingStatus::Moving)
	{
		Timer += DeltaSeconds;
		if (Timer >= RepathInterval)
		{
			// 최신 목표 위치를 다시 가져오기
			const FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
        
			// 새로운 목표 위치로 이동
			AIController->MoveToLocation(TargetLocation, AcceptableRadius);

			// 타이머 초기화
			Timer = 0.0f;
		}
	}

	// 2. 멈춰 있는 경우
	else
	{
		if (PathFollowingComp->DidMoveReachGoal())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
}

EBTNodeResult::Type UBTTask_MoveToLocationAndRepath::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 만약 이 태스크가 다른 이유로 중단되면, AI의 움직임을 즉시 멈춥니다.
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController && AIController->GetPathFollowingComponent())
	{
		AIController->GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
	}
	return EBTNodeResult::Aborted;
}
