// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

struct FAbilityEndedData;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class DON_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Attack();

	UPROPERTY(EditAnywhere)
	bool bRotateToTarget = false;

	UPROPERTY(EditAnywhere)
	FGameplayTag AttackTag;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void RotateToTarget(UBehaviorTreeComponent& OwnerComp, AActor* TargetActor);
};
