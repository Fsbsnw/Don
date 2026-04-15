// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SubSystem/DungeonSubsystem.h"

#include "Actor/PatrolPath.h"
#include "Character/Enemy/DonEnemy.h"
#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "Inn/DonInnLibrary.h"
#include "Inn/Object/InnCustomerGroup.h"
#include "Kismet/GameplayStatics.h"

void UDungeonSubsystem::InitializeData()
{
	UInnManagerSubsystem* InnSystem = GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	if (InnSystem)
	{
		LodgerGroups = InnSystem->GetLodgersArray();
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, APatrolPath::StaticClass(), Actors);

	Paths.Empty();
	
	for (AActor* Actor : Actors)
	{
		if (APatrolPath* Path = Cast<APatrolPath>(Actor))
		{
			Paths.Add(Path);
		}
	}
}

void UDungeonSubsystem::SpawnLodgerEnemy()
{
	int32 Count = 0;
	for (UInnCustomerGroup* LodgerGroup : LodgerGroups)
	{
		for (const FCustomerSnapshot& Member : LodgerGroup->MemberSnapshots)
		{
			FCustomerData LodgerData = UDonInnLibrary::GetCustomerAssetData(this, Member.Type);

			FVector RandomOffset(
				FMath::FRandRange(-50.f, 50.f),
				FMath::FRandRange(-50.f, 50.f),
				0.f
			);
			
			const FVector SpawnLocation = Paths[PathIndex]->GetActorLocation();
			
			FTransform SpawnTransform(
				FRotator::ZeroRotator,
				SpawnLocation + RandomOffset
			);

			if (ADonEnemy* LodgerEnemy = GetWorld()->SpawnActorDeferred<ADonEnemy>(LodgerData.LodgerEnemyClass, SpawnTransform))
			{
				UE_LOG(LogTemp, Warning, TEXT("Lodger Spawned %d"), ++Count);
				LodgerEnemy->GroupID = LodgerGroup->GroupID;
				LodgerEnemy->PatrolPath = Paths[PathIndex];
				LodgerEnemy->SetCharacterLevel(Member.Level);
				LodgerEnemy->OnDungeonGroupKilled.AddUObject(this, &UDungeonSubsystem::CalculateDeadMembers);
				LodgerEnemy->FinishSpawning(SpawnTransform);
			}
		}
		PathIndex = (PathIndex + 1) % Paths.Num();
	}
	LevelEnemyCount = Count;
	LevelEnemyKillCount = 0;
}

void UDungeonSubsystem::CalculateDeadMembers(int32 GroupID)
{
	++LevelEnemyKillCount;
	for (UInnCustomerGroup* Group : LodgerGroups)
	{
		if (Group->GroupID == GroupID)
		{
			Group->DeadMemberCount++;
			return;
		}
	}
}
