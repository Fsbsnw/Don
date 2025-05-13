// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DonAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class DON_API ADonAIController : public AAIController
{
	GENERATED_BODY()
public:
	ADonAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent; 
};
