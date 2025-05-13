// Copyright Epic Games, Inc. All Rights Reserved.


#include "DonGameModeBase.h"

#include "Character/Enemy/DonEnemy.h"

void ADonGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (const TTuple<float, TSubclassOf<ADonEnemy>>& BossSpawnInfo : BossSpawnTimer)
	{
		float SpawnTime = BossSpawnInfo.Key;
		TSubclassOf<ADonEnemy> BossClass = BossSpawnInfo.Value;
		
		FTimerDelegate BossTimerDelegate;
		BossTimerDelegate.BindUFunction(this, FName("SpawnBoss"), SpawnTime, BossClass);

		FTimerHandle BossTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(BossTimerHandle, BossTimerDelegate, SpawnTime, false);
	}
}

void ADonGameModeBase::SpawnBoss(float SpawnTime, TSubclassOf<ADonEnemy> Boss)
{
	if (Boss != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Time : %f, Boss Name : %s, Current Play Time : %f"), SpawnTime, *Boss->GetName(), GetWorld()->GetTimeSeconds());

		if (ADonEnemy* BossActor = GetWorld()->SpawnActor<ADonEnemy>(Boss))
		{
			BossActor->SetCharacterLevel(5.f);
		}

		if (GetWorld()->GetTimeSeconds() < 30.f)
		{
			FTimerDelegate BossTimerDelegate;
			BossTimerDelegate.BindUFunction(this, FName("SpawnBoss"), SpawnTime, Boss);

			FTimerHandle BossTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(BossTimerHandle, BossTimerDelegate, SpawnTime, false);
		}
	}
}
