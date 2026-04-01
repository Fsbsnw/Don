// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Actor/InnTable.h"

#include "Inn/Actor/InnSeat.h"

AInnTable::AInnTable()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInnTable::RegisterSeats(int32 InTableNumber, int32 InRequiredLevel)
{
	TableNumber = InTableNumber;
	RequiredInteriorLevel = InRequiredLevel;
	
	TArray<AActor*> FoundActors;
	GetAllChildActors(FoundActors);

	for (AActor* ChildActor : FoundActors)
	{
		if (AInnSeat* Seat = Cast<AInnSeat>(ChildActor))
		{
			Seat->SeatGroup = InTableNumber;
			Seats.AddUnique(Seat);
		}
	}
}

void AInnTable::SetAvailable(bool NewState)
{
	bIsUpgraded = NewState;

	for (AInnSeat* Seat : Seats)
	{
		Seat->bIsAvailable = NewState;
	}
}
