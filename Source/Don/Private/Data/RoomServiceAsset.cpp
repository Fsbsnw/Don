// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RoomServiceAsset.h"

FRoomServiceOrder URoomServiceAsset::FindRoomServiceByName(FName RoomServiceName)
{
	for (FRoomServiceOrder RoomServiceOrder : RoomServiceInformation)
	{
		if (RoomServiceOrder.ServiceName == RoomServiceName) return RoomServiceOrder;
	}
	return FRoomServiceOrder();
}