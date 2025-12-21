// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_FinishMeal.h"

#include "AIController.h"
#include "Inn/Character/InnCustomer.h"

UBTTask_FinishMeal::UBTTask_FinishMeal()
{
	NodeName = "Finish Meal";
}

EBTNodeResult::Type UBTTask_FinishMeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		Customer->FinishMeal();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}