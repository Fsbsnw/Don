// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_CalculateAndPay.h"

#include "AIController.h"
#include "DonInnGameMode.h"
#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"
#include "Inn/Character/InnCustomer.h"
#include "Kismet/GameplayStatics.h"

UBTTask_CalculateAndPay::UBTTask_CalculateAndPay()
{
	NodeName = "Calculate And Pay";
}

EBTNodeResult::Type UBTTask_CalculateAndPay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		if (ADonInnGameMode* InnGameMode = Cast<ADonInnGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			UKitchenOrderSubsystem* KitchenSystem = GetWorld()->GetGameInstance()->GetSubsystem<UKitchenOrderSubsystem>();
			FCompletedFoodOrder* CompletedOrder = KitchenSystem->GetCompletedOrder(Customer->GetID());
			
			int32 Level = CompletedOrder->ChefLevel;
			int32 Satisfaction = CompletedOrder->CustomerSatisfaction;
			int32 Price = CompletedOrder->FoodPrice + (Satisfaction * 5);
			int32 Reputation = InnGameMode->GetReputation();
			int32 Popularity = InnGameMode->GetPopularity();
			int32 Interior = InnGameMode->GetInterior();

			int32 FinalPrice = Level * Price + 10 * (Reputation + Popularity + Interior);
			CompletedOrder->FoodPrice = FinalPrice;
			
			UE_LOG(LogTemp, Warning, TEXT("Level : %d, Price : %d, Pay : %d"), Level, Price, FinalPrice);
			
			InnGameMode->AddToRevenue(FinalPrice);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
