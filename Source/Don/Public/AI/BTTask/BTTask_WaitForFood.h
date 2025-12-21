// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WaitForFood.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTTask_WaitForFood : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_WaitForFood();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
