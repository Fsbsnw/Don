// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Character/InnCustomer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/CuisineAsset.h"
#include "GameInstance/DonGameInstance.h"
#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"
#include "Inn/DonInnLibrary.h"
#include "Inn/Actor/InnSeat.h"
#include "Inventory/DonItemLibrary.h"

AInnCustomer::AInnCustomer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInnCustomer::BeginPlay()
{
	Super::BeginPlay();

	ID = FGuid::NewGuid();
}

void AInnCustomer::CreateFoodOrder()
{
	if (UDonGameInstance* DGI = Cast<UDonGameInstance>(GetGameInstance()))
	{
		OrderedFood = DGI->GetRandomCuisine();
		OrderedFood.OrderedCustomer = this;
	}
}

void AInnCustomer::OrderFood()
{
	if (MealState == ECustomerMealState::FinishedEating) return;
	UKitchenOrderSubsystem* KitchenSystem = GetGameInstance()->GetSubsystem<UKitchenOrderSubsystem>();
	CreateFoodOrder();
	KitchenSystem->EnqueueKitchenOrder(OrderedFood);	
}

void AInnCustomer::ReceiveFood()
{
	MealState = ECustomerMealState::Eating;

	const FVector FoodSpawnLocation = 
	GetActorLocation()
	+ GetActorForwardVector() * FoodSpawnOffset.X
	+ GetActorRightVector()   * FoodSpawnOffset.Y
	+ FoodSpawnOffset.Z;
	
	FTransform SpawnTransform(FRotator(), FoodSpawnLocation);
	if (AActor* FoodActor = GetWorld()->SpawnActor<AActor>(OrderedFood.FoodActor, SpawnTransform))
	{
		ReceivedFoodActor = FoodActor;
	}
	ReceiveFoodUI();

	UE_LOG(LogTemp, Log, TEXT("%s has received the meal."), *GetName());
}

void AInnCustomer::FinishMeal()
{
	UE_LOG(LogTemp, Log, TEXT("%s has finished the meal."), *GetName());
	OnCustomerChanged.Broadcast(ECustomerNotify::FinishedEating);
}

void AInnCustomer::OnGroupMealFinished(bool State)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("bAllFinishedEating"), State);

		MealState = ECustomerMealState::FinishedEating;
		SeatState = ECustomerSeatState::Idle;

		if (Seat)
		{
			Seat->SetIsOccupied(false);
			Seat = nullptr;
		}
		if (ReceivedFoodActor) ReceivedFoodActor->Destroy();
	}
}

int32 AInnCustomer::GetFoodPrice() const
{
	return OrderedFood.Price;
}

void AInnCustomer::OnSeatAssigned(bool State)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("bFoundEmptySeat"), State);
	}
}

void AInnCustomer::OnGroupDecidedToStay(bool State)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("bUseRoom"), State);
	}
}

void AInnCustomer::EnterRoom()
{
	OnCustomerChanged.Broadcast(ECustomerNotify::EnterRoom);
}

void AInnCustomer::SetDestination(const ECustomerInnState& Destination)
{
	UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	if (InnSystem == nullptr) return;

	switch (Destination)
	{
	case ECustomerInnState::Entrance:
		{
			InnState = ECustomerInnState::Entrance;
			FVector RandomOffset(
			FMath::FRandRange(-100.f, 100.f),
			FMath::FRandRange(-100.f, 100.f),
			0.f
			);
			NextDestination = InnSystem->InnEntranceLocation + RandomOffset;
			break;
		}
		
	case ECustomerInnState::Kitchen:
		{
			if (Seat != nullptr)
			{
				InnState = ECustomerInnState::Kitchen;
				SeatState = ECustomerSeatState::MoveToSeat;
				NextDestination = Seat->GetActorLocation();			
			}
			break;
		}
		
	case ECustomerInnState::Room:
		{
			InnState = ECustomerInnState::Room;
			SeatState = ECustomerSeatState::Idle;
			NextDestination = InnSystem->RoomEntranceLocation;
			break;
		}
		
	case ECustomerInnState::Exit:
		{
			InnState = ECustomerInnState::Exit;
			NextDestination = InnSystem->ExitLocation;
			OnInnGroupStateChanged.Clear();
			InnSystem->RemoveGroup(GroupID);
			break;
		}
	}
	if (Destination != ECustomerInnState::Exit)	OnInnGroupStateChanged.Broadcast(GroupID, Destination);
}

void AInnCustomer::EnterInn()
{
	OnCustomerChanged.Broadcast(ECustomerNotify::EnterInn);
}

void AInnCustomer::ExitInn()
{
	OnCustomerChanged.Broadcast(ECustomerNotify::ExitInn);
}

void AInnCustomer::ReserveSeat(AInnSeat* NewSeat)
{
	if (MealState == ECustomerMealState::FinishedEating) return;

	Seat = NewSeat;
}

void AInnCustomer::SitOnSeat()
{
	if (Seat == nullptr) return;
	
	SeatState = ECustomerSeatState::Sit;

	SetActorLocation(Seat->SitPoint->GetComponentLocation());
	SetActorRotation(Seat->SitPoint->GetComponentRotation());
}