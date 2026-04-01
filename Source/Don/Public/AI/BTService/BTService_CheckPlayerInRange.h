// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UBTService_CheckPlayerInRange : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckPlayerInRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float TargetRadius = 300.f;
};
