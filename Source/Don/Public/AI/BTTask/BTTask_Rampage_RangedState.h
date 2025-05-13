// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Rampage_RangedState.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTTask_Rampage_RangedState : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Rampage_RangedState();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
