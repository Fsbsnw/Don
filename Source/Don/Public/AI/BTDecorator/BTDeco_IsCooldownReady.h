// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDeco_IsCooldownReady.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTDeco_IsCooldownReady : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDeco_IsCooldownReady();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "Cooldown")
	FGameplayTag CooldownTag;
};
