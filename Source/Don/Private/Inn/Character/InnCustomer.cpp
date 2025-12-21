// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Character/InnCustomer.h"

#include "Data/CuisineAsset.h"
#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"
#include "Inn/DonInnLibrary.h"
#include "Inn/Actor/InnChef.h"
#include "Inn/Actor/InnSeat.h"
#include "Kismet/GameplayStatics.h"

AInnCustomer::AInnCustomer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInnCustomer::OrderFood()
{
	const int32 Index = FMath::RandRange(0, FavoriteFoods.Num() - 1);
	FoodOrder = UDonInnLibrary::FindCuisineByName(this, FavoriteFoods[Index]);
	FoodOrder.OrderedCustomer = this;
	UDonInnLibrary::AddKitchenOrder(this, FoodOrder);
}

void AInnCustomer::ReceivedFood()
{
	MealState = ECustomerMealState::Eating;

	UE_LOG(LogTemp, Log, TEXT("%s has received the meal."), *GetName());
}

void AInnCustomer::FinishMeal()
{
	Seat->SetIsOccupied(false);
	Seat = nullptr;
	
	MealState = ECustomerMealState::FinishedEating;
	SeatState = ECustomerSeatState::Idle;

	UE_LOG(LogTemp, Log, TEXT("%s has finished the meal."), *GetName());
}

void AInnCustomer::EnterRoom()
{
	// FInnCustomer 만들고 정보 저장 및 Customer 서브 시스템에 등록해야 할 듯
	Destroy();
}

FVector AInnCustomer::SetDestination(const ECustomerInnState Destination)
{
	const UKitchenOrderSubsystem* KitchenOrderSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UKitchenOrderSubsystem>();
	if (KitchenOrderSubsystem == nullptr) return FVector::ZeroVector;
	
	if (Destination == ECustomerInnState::Kitchen && Seat != nullptr)
	{
		InnState = ECustomerInnState::Kitchen;
		SeatState = ECustomerSeatState::MoveToSeat;
		
		return Seat->GetActorLocation();
	}
	if (Destination == ECustomerInnState::Room)
	{
		InnState = ECustomerInnState::Room;

		return KitchenOrderSubsystem->RoomEntranceLocation;
	}
	if (Destination == ECustomerInnState::Exit)
	{
		InnState = ECustomerInnState::Exit;

		return KitchenOrderSubsystem->ExitLocation;
	}

	return FVector::ZeroVector;
}

int32 AInnCustomer::GetChefLevel() const
{
	if (FoodOrder.AssignedChef) return FoodOrder.AssignedChef->GetChefLevel();
	return 0;
}

void AInnCustomer::RequestSeat()
{
	if (MealState == ECustomerMealState::FinishedEating) return;
	
	UKitchenOrderSubsystem* KitchenSystem = GetGameInstance()->GetSubsystem<UKitchenOrderSubsystem>();
	AInnSeat* NewSeat = KitchenSystem->FindAndOccupyEmptySeat();

	if (NewSeat == nullptr) return;

	Seat = NewSeat;
}

void AInnCustomer::SitOnSeat()
{
	SeatState = ECustomerSeatState::Sit;

	SetActorLocation(Seat->GetActorLocation());
	SetActorRotation(Seat->GetActorRotation());
}