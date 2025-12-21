// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_CalculateAndPay.h"

#include "AIController.h"
#include "Inn/Character/InnCustomer.h"

UBTTask_CalculateAndPay::UBTTask_CalculateAndPay()
{
	NodeName = "Calculate And Pay";
}

EBTNodeResult::Type UBTTask_CalculateAndPay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		int32 Level = Customer->GetChefLevel();
		int32 Price = Customer->GetFoodPrice();

		UE_LOG(LogTemp, Warning, TEXT("Level : %d, Price : %d, Pay : %d"), Level, Price, Level * Price);
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
