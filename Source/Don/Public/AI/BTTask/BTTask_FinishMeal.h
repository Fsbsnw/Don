// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FinishMeal.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTTask_FinishMeal : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FinishMeal();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
