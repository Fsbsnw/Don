// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDeco_IsFoodServed.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTDeco_IsFoodServed : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDeco_IsFoodServed();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
