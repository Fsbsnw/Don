// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_OrderFood.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTTask_OrderFood : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_OrderFood();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
