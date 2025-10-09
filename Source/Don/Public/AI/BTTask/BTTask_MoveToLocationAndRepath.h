// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToLocationAndRepath.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTTask_MoveToLocationAndRepath : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_MoveToLocationAndRepath();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float AcceptableRadius = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float RepathInterval = 1.0f; 

private:
	float Timer = 0.f;
};
