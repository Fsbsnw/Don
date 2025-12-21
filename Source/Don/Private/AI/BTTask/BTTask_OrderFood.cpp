// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_OrderFood.h"

#include "AIController.h"
#include "Inn/Character/InnCustomer.h"

UBTTask_OrderFood::UBTTask_OrderFood()
{
	NodeName = "Order Food";
}

EBTNodeResult::Type UBTTask_OrderFood::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		Customer->OrderFood();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
