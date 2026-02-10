// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_FinishMeal.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"
#include "Inn/Character/InnCustomer.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FinishMeal::UBTTask_FinishMeal()
{
	NodeName = "Finish Meal";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FinishMeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		Customer->FinishMeal();
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_FinishMeal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	bool bAllFinishedEating = OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsBool(FName("bAllFinishedEating"));

	if (bAllFinishedEating)	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
