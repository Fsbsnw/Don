// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Inn/Character/InnCustomer.h"
#include "BTTask_MoveToDestination.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTTask_MoveToDestination : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_MoveToDestination();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere)
	ECustomerInnState Destination = ECustomerInnState::Kitchen;
};