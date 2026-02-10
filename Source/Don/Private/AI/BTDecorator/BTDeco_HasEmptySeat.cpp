// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDeco_HasEmptySeat.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDeco_HasEmptySeat::UBTDeco_HasEmptySeat()
{
	NodeName = TEXT("Has Empty Seat");
}

bool UBTDeco_HasEmptySeat::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return false; 
}