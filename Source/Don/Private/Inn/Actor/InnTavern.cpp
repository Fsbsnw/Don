// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Actor/InnTavern.h"

#include "Inn/Actor/InnTable.h"
#include "Inn/InnManagerComponent/InnManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"

AInnTavern::AInnTavern()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AInnTavern::RegisterTables()
{
	if (ADonPlayerState* DPC = Cast<ADonPlayerState>(UGameplayStatics::GetPlayerState(this, 0)))
	{
		DPC->InnManagerComponent->OnInnLevelChanged.AddUObject(this, &AInnTavern::UpgradeTavernInterior);
	}
	
	TArray<AActor*> ChildActors;
	GetAllChildActors(ChildActors);

	for (AActor* Child : ChildActors)
	{
		if (AInnTable* Table = Cast<AInnTable>(Child))
		{
			Tables.AddUnique(Table);
		}
	}
}

void AInnTavern::UpgradeTavernInterior(int32 InteriorLevel)
{
	for (AInnTable* Table : Tables)
	{
		if (Table->bIsUpgraded) continue;

		if (Table->RequiredInteriorLevel <= InteriorLevel)
		{
			Table->SetAvailable(true);
			Table->OnTableUpgraded(true);
		}
		else
		{
			Table->OnTableUpgraded(false);
		}
	}
}

void AInnTavern::BroadcastInitialTables()
{
	if (ADonPlayerState* DPC = Cast<ADonPlayerState>(UGameplayStatics::GetPlayerState(this, 0)))
	{
		UpgradeTavernInterior(DPC->InnManagerComponent->GetInnLevel());
	}
}
