// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SubSystem/RoomServiceOrderSubsystem.h"

bool URoomServiceOrderSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (const UWorld* World = Outer->GetWorld())
	{
		return World->GetMapName().Contains("InnMap");
	}
	return false;
}

void URoomServiceOrderSubsystem::BroadcastInitialValues()
{
	for (const FRoomServiceOrder& Order : RoomServiceOrderQueue)
	{
		OnRoomServiceOrderChanged.Broadcast(Order);
	}
}

FRoomServiceOrder URoomServiceOrderSubsystem::EnqueueRoomServiceOrder(const FRoomServiceOrder& Order)
{
	RoomServiceOrderQueue.Add(Order);
	OnRoomServiceOrderChanged.Broadcast(Order);
	return Order;
}

FRoomServiceOrder URoomServiceOrderSubsystem::DequeueRoomServiceOrder()
{
	if (RoomServiceOrderQueue.IsEmpty()) return FRoomServiceOrder();
	
	const FRoomServiceOrder Order = RoomServiceOrderQueue[0];
	RoomServiceOrderQueue.RemoveAt(0);
	OnRoomServiceOrderChanged.Broadcast(Order);
	return Order;
}