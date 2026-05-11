// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Component/InnManagerComponent.h"

#include "DonInnGameMode.h"
#include "Inn/Actor/InnChef.h"
#include "Inn/Actor/InnChefPoint.h"
#include "Inn/Component/InnStoreComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"

void UInnManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	ADonPlayerState* DPS = CastChecked<ADonPlayerState>(GetOwner());
	if (DPS && DPS->InnStoreComponent)
	{
		DPS->InnStoreComponent->OnUpgradeRequested.AddUObject(this, &UInnManagerComponent::HandleUpgradeRequest);
	}
}

UInnManagerComponent::UInnManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInnManagerComponent::SavePlayerData(FPlayerSaveData& Data)
{
	Data.InnLevel = InnLevel;
	Data.Interior = Interior;
	Data.Popularity = Popularity;
	Data.Reputation = Reputation;
	Data.Suspicion = Suspicion;
	
	for (int32 i = 0; i < 3; ++i)
	{
		if (!Chefs[i]) continue;
		
		const AInnChef* Chef = Chefs[i];
		ChefSaveData[i].ChefLevel = Chef->GetChefLevel();
		ChefSaveData[i].bIsHired = Chef->bIsHired;
		ChefSaveData[i].ChefXP = Chef->ChefXP;
	}
	for (int32 i = 0; i < 3; ++i)
		Data.ChefData[i] = ChefSaveData[i];
}

void UInnManagerComponent::LoadPlayerData(const FPlayerSaveData& InData)
{
	InnLevel = InData.InnLevel;
	Interior = InData.Interior;
	Popularity = InData.Popularity;
	Reputation = InData.Reputation;
	Suspicion = InData.Suspicion;

	for (int32 i = 0; i < 3; ++i)
	{
		ChefSaveData[i] = InData.ChefData[i];
	}
	
	FString MapName = GetWorld()->GetMapName();
	if (!MapName.Contains(TEXT("InnMap"))) return;
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, AInnChefPoint::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		if (AInnChefPoint* Point = Cast<AInnChefPoint>(Actor))
		{
			int32 i = Point->PointIndex;
			const TSubclassOf<AInnChef>& TChef = ChefsClasses[i];

			if (AInnChef* Chef = GetWorld()->SpawnActor<AInnChef>(TChef, Point->GetActorTransform()))
			{
				Chef->ChefLevel = ChefSaveData[i].ChefLevel;
				Chef->ChefXP = ChefSaveData[i].ChefXP;
				if (i == 0) Chef->Hired(true);
				else Chef->Hired(ChefSaveData[i].bIsHired);
				Chefs[i] = Chef;
			}
		}
	}

	OnInnLevelChanged.Broadcast(InnLevel);
}

void UInnManagerComponent::AddToReputation(int32 InReputation)
{
	Reputation += InReputation;
	OnReputationChanged.Broadcast(Reputation);
}

void UInnManagerComponent::AddToPopularity(int32 InPopularity)
{
	Popularity += InPopularity;
	OnPopularityChanged.Broadcast(Popularity);
}

void UInnManagerComponent::AddToInterior(int32 InInterior)
{
	Interior += InInterior;
	OnInteriorChanged.Broadcast(Interior);
}

void UInnManagerComponent::AddToSuspicion(int32 InSuspicion)
{
	Suspicion += InSuspicion;
	OnSuspicionChanged.Broadcast(Suspicion);

	if (Suspicion >= 300)
	{
		if (ADonInnGameMode* InnGameMode = Cast<ADonInnGameMode>(GetWorld()->GetAuthGameMode()))
		{
			InnGameMode->GameOver(1);
		}
	}
}

void UInnManagerComponent::AddToInnLevel(int32 InInnLevel)
{
	InnLevel += InInnLevel;
	OnInnLevelChanged.Broadcast(InnLevel);
}

void UInnManagerComponent::HandleUpgradeRequest(bool bTargetIsTable)
{
	if (bTargetIsTable)
	{
		UpgradeInn();
	}
	else
	{
		HireNewChef();
	}
}

bool UInnManagerComponent::HireNewChef()
{
	for (AInnChef* Chef : Chefs)
	{
		if (!Chef->bIsHired)
		{
			Chef->Hired(true);
			return true;
		}		
	}
	return false;
}

void UInnManagerComponent::UpgradeInn()
{
	AddToInnLevel(1);
}