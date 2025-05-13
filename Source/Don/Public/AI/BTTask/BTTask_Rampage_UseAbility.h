// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Rampage_UseAbility.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTTask_Rampage_UseAbility : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Rampage_UseAbility();
	void RotateToTarget(UBehaviorTreeComponent& OwnerComp, AActor* TargetActor);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
public:
	UPROPERTY(EditAnywhere, Category = "Ability Tag")
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, Category = "Ability Tag")
	bool bRotateToTarget = false;
};
