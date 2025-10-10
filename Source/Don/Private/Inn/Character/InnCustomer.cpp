// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Character/InnCustomer.h"

#include "AI/DonAIController.h"
#include "Data/CuisineAsset.h"
#include "Inn/DonInnLibrary.h"
#include "Inn/Actor/InnSeat.h"

AInnCustomer::AInnCustomer()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AInnCustomer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInnCustomer::OrderFood()
{
	if (FavoriteFoods.IsEmpty()) return;
	
	const int32 Index = FMath::RandRange(0, FavoriteFoods.Num() - 1);
	FKitchenOrder FoodOrder = UDonInnLibrary::FindCuisineByName(this, FavoriteFoods[Index]);
	FoodOrder.OrderedCustomer = this;
	UDonInnLibrary::AddKitchenOrder(this, FoodOrder);
}

void AInnCustomer::ReceivedFood()
{
	MealState = ECustomerMealState::Eating;
	
	FTimerHandle MealTimerHandle;
	GetWorldTimerManager().SetTimer(
		MealTimerHandle,
		this,               
		&AInnCustomer::OnMealFinished, 
		EatingTime,                
		false                  
	);

	UE_LOG(LogTemp, Log, TEXT("%s has received the meal."), *GetName());
}

void AInnCustomer::OnMealFinished()
{
	MealState = ECustomerMealState::FinishedEating;

	ADonAIController* DonAIController = Cast<ADonAIController>(GetController());
	if (DonAIController == nullptr) return;
		
	// 여관방 빈 자리 있는 경우
	if (true)
	{
		InnState = ECustomerInnState::Room;

		DonAIController->MoveToLocation(RoomEntrance->GetActorLocation());
	}
	else
	{
		InnState = ECustomerInnState::Exit;
		
		DonAIController->MoveToLocation(Exit->GetActorLocation());
	}

	Seat = nullptr;
	
	UE_LOG(LogTemp, Log, TEXT("%s has finished the meal."), *GetName());
}

void AInnCustomer::EnterRoom()
{
	// FInnCustomer 만들고 정보 저장 및 Customer 서브 시스템에 등록해야 할 듯
	Destroy();
}