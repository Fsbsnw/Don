// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_MoveToDestination.h"

#include "AIController.h"
#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
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
		Customer->SetDestination(Destination);
		FVector TargetLocation = Customer->GetDestination();
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
		EPathFollowingStatus::Type Status = AIController->GetMoveStatus();

		if (Status == EPathFollowingStatus::Idle)
		{
			APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
			if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
			{
				if (Customer->GetInnState() == ECustomerInnState::Entrance)	Customer->EnterInn();
				else if (Customer->GetInnState() == ECustomerInnState::Room) Customer->EnterRoom();
				else if (Customer->GetInnState() == ECustomerInnState::Exit) Customer->ExitInn();
			}
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}