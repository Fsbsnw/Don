// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SitOnSeat.h"

#include "AIController.h"
#include "Inn/Character/InnCustomer.h"

UBTTask_SitOnSeat::UBTTask_SitOnSeat()
{
	NodeName = "Sit On Seat";
}

EBTNodeResult::Type UBTTask_SitOnSeat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		Customer->SitOnSeat();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}