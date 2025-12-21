// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDeco_IsFoodServed.h"

#include "AIController.h"
#include "Inn/Character/InnCustomer.h"

UBTDeco_IsFoodServed::UBTDeco_IsFoodServed()
{
	NodeName = "Is Food Served";
}

bool UBTDeco_IsFoodServed::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (const AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		return Customer->GetMealState() >= ECustomerMealState::Eating;
	}

	return false;
}
