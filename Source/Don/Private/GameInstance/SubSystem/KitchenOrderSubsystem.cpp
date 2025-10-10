// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"

#include "Inn/Actor/InnChef.h"
#include "Inn/Actor/InnSeat.h"
#include "Inn/Character/InnCustomer.h"

bool UKitchenOrderSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (const UWorld* World = Outer->GetWorld())
	{
		return World->GetMapName().Contains("InnMap");
	}
	return false;
}

void UKitchenOrderSubsystem::BroadcastInitialValues()
{
	OnKitchenOrderUpdated.Broadcast(KitchenOrderQueue);
}

FKitchenOrder UKitchenOrderSubsystem::EnqueueKitchenOrder(FKitchenOrder& Order)
{
	Order.OrderID = FGuid::NewGuid();
	Order.RemainingTime = Order.CookingTime;
	
	// 요리사 여유가 있는 경우
	if (AInnChef* IdleChef = FindIdleChef())
	{
		Order.AssignedChef = IdleChef;
		IdleChef->StartOrder(Order);
		if (!GetWorld()->GetTimerManager().IsTimerActive(OrderTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				OrderTimerHandle,
				this,
				&UKitchenOrderSubsystem::UpdateKitchenOrders,
				1.0f,
				true
			);
		}
	}
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

AInnSeat* UKitchenOrderSubsystem::FindEmptySeat(AInnCustomer* Customer)
{
	if (Customer == nullptr) return nullptr;
	
	for (AInnSeat* Seat : Seats)
	{
		if (!Seat->GetIsOccupied()) return Seat;
	}
	return nullptr;
}

void UKitchenOrderSubsystem::AssignSeatToCustomer(AInnCustomer* Customer)
{
	if (AInnSeat* Seat = FindEmptySeat(Customer))
	{
		Seat->SetIsOccupied(true);
		Customer->Seat = Seat;
		UE_LOG(LogTemp, Warning, TEXT("Assigned Seat Location : %s"), *Seat->GetActorLocation().ToString());
	}
}

void UKitchenOrderSubsystem::SpawnKitchenCustomer(TSubclassOf<AInnCustomer> CustomerClass, FVector SpawnLocation)
{
	AInnCustomer* Customer = GetWorld()->SpawnActor<AInnCustomer>(CustomerClass, SpawnLocation, FRotator::ZeroRotator);
	if (Customer)
	{
		AssignSeatToCustomer(Customer);
	}	
}

void UKitchenOrderSubsystem::UpdateKitchenOrders()
{
	TArray<int32> IndexToRemove;
	for (int32 i = 0; i < KitchenOrderQueue.Num(); i++)
	{
		FKitchenOrder& Order = KitchenOrderQueue[i];
		if (Order.bIsCooking && Order.RemainingTime > 0)
		{
			Order.RemainingTime--;

			if (Order.RemainingTime <= 0)
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
		const FKitchenOrder Order = KitchenOrderQueue[i];
		Order.AssignedChef->EndOrder();
		if (AInnCustomer* Customer = Cast<AInnCustomer>(Order.OrderedCustomer))
		{
			Customer->ReceivedFood();
		}
		KitchenOrderQueue.RemoveAt(i);
		OnKitchenOrderRemoved.Broadcast(Order);

		for (FKitchenOrder& NewOrder : KitchenOrderQueue)
		{
			if (!NewOrder.bIsCooking && NewOrder.RemainingTime > 0.f)
			{
				NewOrder.AssignedChef = Order.AssignedChef;
				NewOrder.AssignedChef->StartOrder(NewOrder);
				break;
			}
		}
	}

	if (KitchenOrderQueue.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(OrderTimerHandle);
	}
}
