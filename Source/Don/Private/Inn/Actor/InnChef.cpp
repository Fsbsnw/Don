// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Actor/InnChef.h"

#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"

AInnChef::AInnChef()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetVisibility(false);
	PrimaryActorTick.bCanEverTick = false;
}

void AInnChef::BroadcastInitialAttributes()
{
	OnChefAttributeChanged.Broadcast(GetChefUIData());
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

FChefUIData AInnChef::GetChefUIData() const
{
	FChefUIData UIData;
	UIData.bIsHired = bIsHired;
	UIData.ChefImage = ChefImage;
	UIData.ChefLevel = ChefLevel;
	UIData.ChefXP = ChefXP;
	return UIData;
}

void AInnChef::Hired(bool NewState)
{
	bIsHired = NewState;
	OnChefAttributeChanged.Broadcast(GetChefUIData());
	OnChefHiredChanged(NewState);
}

void AInnChef::StartOrder(FKitchenOrder& Order)
{
	if (Mesh)
	{
		Mesh->SetVisibility(true);
		Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		Mesh->Play(true);
	}
	Order.bIsCooking = true;
	Order.CookingTime = FMath::Max(2, Order.CookingTime - (ChefLevel - 1));
	Order.RemainingTime = Order.CookingTime;
	OrderID = Order.OrderID;
	bIsCooking = true;
}

void AInnChef::EndOrder()
{
	if (Mesh)
	{
		Mesh->SetVisibility(false);
		Mesh->Stop();
	}
	OrderID = FGuid();
	bIsCooking = false;
	OnChefAttributeChanged.Broadcast(GetChefUIData());
}

void AInnChef::ChefLevelUp()
{
	ChefXP = ChefXP - ChefLevel * 100;
	ChefLevel = FMath::Min(20, ChefLevel + 1);
}

void AInnChef::AddToXP(int32 InXP)
{
	ChefXP += InXP;
	if (ChefXP >= ChefLevel * 100) ChefLevelUp();
	OnChefAttributeChanged.Broadcast(GetChefUIData());
}
