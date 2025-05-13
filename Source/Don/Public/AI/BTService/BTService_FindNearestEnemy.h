// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindNearestEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTService_FindNearestEnemy : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_FindNearestEnemy();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
