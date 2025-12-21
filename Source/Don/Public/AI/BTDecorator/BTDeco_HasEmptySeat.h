// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDeco_HasEmptySeat.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTDeco_HasEmptySeat : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDeco_HasEmptySeat();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
