// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"

#include "Inn/Actor/InnChef.h"
#include "Inn/Actor/InnSeat.h"
#include "Inn/Character/InnCustomer.h"
#include "Kismet/GameplayStatics.h"

bool UKitchenOrderSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (const UWorld* World = Outer->GetWorld())
	{
		return World->GetMapName().Contains("InnMap");
	}
	return false;
}

void UKitchenOrderSubsystem::InitDestLocations()
{
	TArray<AActor*> Exits;
	UGameplayStatics::GetAllActorsWithTag(this, FName("Exit"), Exits);

	for (const AActor* Target : Exits) ExitLocation = Target->GetActorLocation();

	
	TArray<AActor*> InnEntrances;
	UGameplayStatics::GetAllActorsWithTag(this, FName("InnEntrance"), InnEntrances);

	for (const AActor* Target : InnEntrances) InnEntranceLocation = Target->GetActorLocation();

	
	TArray<AActor*> RoomEntrances;
	UGameplayStatics::GetAllActorsWithTag(this, FName("RoomEntrance"), RoomEntrances);

	for (const AActor* Target : RoomEntrances) RoomEntranceLocation = Target->GetActorLocation();

	TArray<AActor*> SeatActors;
	UGameplayStatics::GetAllActorsOfClass(this, AInnSeat::StaticClass(), SeatActors);

	for (AActor* Actor : SeatActors)
	{
		if (AInnSeat* Seat = Cast<AInnSeat>(Actor))
		{
			Seats.Add(Seat);
		}
	}
}

void UKitchenOrderSubsystem::BroadcastInitialValues()
{
	OnKitchenOrderUpdated.Broadcast(KitchenOrderQueue);
}

void UKitchenOrderSubsystem::AssignChef(FKitchenOrder& Order)
{
	if (AInnChef* IdleChef = FindIdleChef())
	{
		Order.AssignedChef = IdleChef;
		IdleChef->StartOrder(Order);
		Order.OrderedCustomer->FoodOrder = Order;
		
		if (!GetWorld()->GetTimerManager().IsTimerActive(OrderTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				OrderTimerHandle,
				this,
				&UKitchenOrderSubsystem::UpdateKitchenOrders,
				TickTimer,
				true
			);
		}
	}
}

FKitchenOrder UKitchenOrderSubsystem::EnqueueKitchenOrder(FKitchenOrder& Order)
{
	Order.OrderID = FGuid::NewGuid();
	Order.RemainingTime = Order.CookingTime;
	
	AssignChef(Order);
	
	KitchenOrderQueue.Add(Order);
	OnKitchenOrderAdded.Broadcast(Order);
	return Order;
}

FGuid UKitchenOrderSubsystem::FindNextQueuedOrderID() const
{
	for (const FKitchenOrder& Order : KitchenOrderQueue)
	{
		if (!Order.bIsCooking)
		{
			return Order.OrderID;
		}
	}
	return FGuid();
}

AInnChef* UKitchenOrderSubsystem::FindIdleChef()
{
	for (AInnChef* Chef : Chefs)
	{
		if (!Chef->IsCooking()) return Chef;
	}
	return nullptr;
}

AInnSeat* UKitchenOrderSubsystem::FindAndOccupyEmptySeat()
{
	for (AInnSeat* Seat : Seats)
	{
		if (!Seat->GetIsOccupied())
		{
			Seat->SetIsOccupied(true);
			return Seat;
		}
	}
	return nullptr;
}

bool UKitchenOrderSubsystem::HasEmptySeat()
{
	for (AInnSeat* Seat : Seats)
	{
		if (!Seat->GetIsOccupied()) return true;
	}
	return false;
}

void UKitchenOrderSubsystem::SpawnKitchenCustomer(TSubclassOf<AInnCustomer> CustomerClass, FVector SpawnLocation)
{
	AInnCustomer* Customer = GetWorld()->SpawnActor<AInnCustomer>(CustomerClass, SpawnLocation, FRotator::ZeroRotator);
}

void UKitchenOrderSubsystem::UpdateKitchenOrders()
{
	TArray<int32> IndexToRemove;
	for (int32 i = 0; i < KitchenOrderQueue.Num(); i++)
	{
		FKitchenOrder& Order = KitchenOrderQueue[i];
		if (Order.bIsCooking && Order.RemainingTime > 0)
		{
			Order.RemainingTime -= TickTimer;

			if (Order.RemainingTime <= 0.f)
			{
				Order.bIsCooking = false;
				IndexToRemove.Add(i);
			}
		}
	}

	OnKitchenOrderUpdated.Broadcast(KitchenOrderQueue);

	// 내림차순 제거
	IndexToRemove.Sort(TGreater<int32>());

	for (int32 i : IndexToRemove)
	{
		FKitchenOrder Order = KitchenOrderQueue[i];
		Order.AssignedChef->EndOrder();
		if (AInnCustomer* Customer = Cast<AInnCustomer>(Order.OrderedCustomer))
		{
			Customer->ReceivedFood();
		}

		// 완료된 요리 주문을 제거하기 전에 요리사를 다음 주문에 할당
		for (FKitchenOrder& NewOrder : KitchenOrderQueue)
		{
			if (FindIdleChef() != nullptr &&
				!NewOrder.bIsCooking &&
				NewOrder.RemainingTime > 0.f)
			{
				AssignChef(NewOrder);
				break;
			}
		}
		
		KitchenOrderQueue.RemoveAt(i);
		OnKitchenOrderRemoved.Broadcast(Order);
	}

	if (KitchenOrderQueue.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(OrderTimerHandle);
	}
}