// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Actor/InnChef.h"

#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"

AInnChef::AInnChef()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInnChef::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UKitchenOrderSubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<UKitchenOrderSubsystem>())
		{
			Subsystem->RegisterChef(this);
		}
	}	
}

void AInnChef::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (UKitchenOrderSubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<UKitchenOrderSubsystem>())
		{
			Subsystem->UnregisterChef(this);
		}
	}	
	
	Super::EndPlay(EndPlayReason);
}

void AInnChef::StartOrder(FKitchenOrder& Order)
{
	Order.bIsCooking = true;
	Order.CookingTime = FMath::Max(2, Order.CookingTime - (ChefLevel - 1));
	Order.RemainingTime = Order.CookingTime;
	OrderID = Order.OrderID;
	bIsCooking = true;
}

void AInnChef::EndOrder()
{
	OrderID = FGuid();
	bIsCooking = false;
}
