// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_RequestSeat.h"

#include "AIController.h"
#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"
#include "Inn/Character/InnCustomer.h"
#include "Kismet/GameplayStatics.h"

UBTTask_RequestSeat::UBTTask_RequestSeat()
{
	NodeName = "Request Seat";
}

EBTNodeResult::Type UBTTask_RequestSeat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UKitchenOrderSubsystem* KitchenSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UKitchenOrderSubsystem>();
	if (KitchenSystem == nullptr) return EBTNodeResult::Failed;

	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		Customer->RequestSeat();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
