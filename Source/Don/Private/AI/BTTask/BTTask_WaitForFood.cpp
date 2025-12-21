// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_WaitForFood.h"

#include "AIController.h"
#include "Inn/Character/InnCustomer.h"

UBTTask_WaitForFood::UBTTask_WaitForFood()
{
	NodeName = "Wait For Food";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_WaitForFood::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTask_WaitForFood::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		if (Customer->GetMealState() == ECustomerMealState::Eating)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}