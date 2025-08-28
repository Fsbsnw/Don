// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Character/InnCustomer.h"

#include "Data/CuisineAsset.h"
#include "Inn/DonInnLibrary.h"

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
	
	int32 Index = FMath::RandRange(0, FavoriteFoods.Num() - 1);
	FKitchenOrder FoodOrder = FavoriteFoods[Index];
	UDonInnLibrary::AddKitchenOrder(this, FoodOrder);

	UE_LOG(LogTemp, Log, TEXT("%d, %d"), Index, FavoriteFoods.Num());
}

void AInnCustomer::ReceivedFood()
{
	MealState = ECustomerMealState::Eating;
	
	FTimerHandle MealTimerHandle;
	GetWorldTimerManager().SetTimer(
		MealTimerHandle,
		this,               
		&AInnCustomer::OnMealFinished, 
		10.0f,                
		false                  
	);
}

void AInnCustomer::OnMealFinished()
{
	MealState = ECustomerMealState::FinishedEating;
	
	UE_LOG(LogTemp, Log, TEXT("%s has finished the meal."), *GetName());
}

void AInnCustomer::EnterRoom()
{
	// FInnCustomer 만들고 정보 저장 및 Customer 서브 시스템에 등록해야 할 듯
	Destroy();
}