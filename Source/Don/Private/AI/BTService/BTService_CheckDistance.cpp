// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_CheckDistance.h"

UBTService_CheckDistance::UBTService_CheckDistance()
{
	NodeName = TEXT("Check Distance");
}

void UBTService_CheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
}
