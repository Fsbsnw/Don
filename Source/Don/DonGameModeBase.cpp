// Copyright Epic Games, Inc. All Rights Reserved.


#include "DonGameModeBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"
#include "Character/Enemy/DonEnemy.h"
#include "Character/NPC/MerchantNPC.h"
#include "Character/NPC/NPCCharacterBase.h"
#include "GameState/DonGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"

void ADonGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (!bStartLevel) return;

	for (FEnemySpawnInfo EnemySpawnInfo : EnemySpawnSchedule)
	{
		// Spawn Enemy
		if (EnemySpawnInfo.EnemyClass->IsChildOf(ADonEnemy::StaticClass()))
		{
			float SpawnDelay = EnemySpawnInfo.SpawnTime;
			TSubclassOf<ADonEnemy> EnemyClass = TSubclassOf<ADonEnemy>(EnemySpawnInfo.EnemyClass);
			int32 Amount = EnemySpawnInfo.Amount;
			
			FTimerDelegate EnemyTimerDelegate;
			EnemyTimerDelegate.BindUFunction(this, FName("SpawnEnemy"), SpawnDelay, EnemyClass, Amount);

			FTimerHandle EnemyTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(EnemyTimerHandle, EnemyTimerDelegate, SpawnDelay, false);


			// Bonus Enemy
			FTimerDelegate BonusEnemyTimerDelegate;
			BonusEnemyTimerDelegate.BindUFunction(this, FName("SpawnBonusEnemy"),EnemyClass);

			FTimerHandle BonusEnemyTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(BonusEnemyTimerHandle, BonusEnemyTimerDelegate, BonusEnemySpawnTime, true);
		}
		// Spawn NPC
		else if (EnemySpawnInfo.EnemyClass->IsChildOf(ANPCCharacterBase::StaticClass()))
		{
			float SpawnDelay = EnemySpawnInfo.SpawnTime;
			TSubclassOf<ANPCCharacterBase> NPCClass = TSubclassOf<ANPCCharacterBase>(EnemySpawnInfo.EnemyClass);
			
			FTimerDelegate NPCTimerDelegate;
			NPCTimerDelegate.BindUFunction(this, FName("SpawnNPC"), NPCClass);

			FTimerHandle NPCTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(NPCTimerHandle, NPCTimerDelegate, SpawnDelay, true);
		}
	}
}

void ADonGameModeBase::AddToSpawnedEnemies(int32 Value)
{
	SpawnedEnemyAmount += Value;
	SpawnedEnemyAmount = FMath::Max(SpawnedEnemyAmount, 0);
	OnSpawnedAmountDelegate.Broadcast(SpawnedEnemyAmount);
}

void ADonGameModeBase::SpawnBonusEnemy(TSubclassOf<ACharacter> Enemy)
{
	if (bStopLevel || SpawnedEnemyAmount >= MaxEnemyEntities) return;
	
	for (APlayerState* PS : GameState->PlayerArray)
	{
		ADonPlayerState* DonPS = Cast<ADonPlayerState>(PS);
		if (!DonPS) continue;

		UAbilitySystemComponent* ASC = DonPS->GetAbilitySystemComponent();
		if (!ASC) continue;
		
		const UDonAttributeSet* DonAttributeSet = ASC->GetSet<UDonAttributeSet>();
		if (!DonAttributeSet) continue;

		float EnemySpawnChance = DonAttributeSet->GetSpawnChance();
		float AdjustedChance = BonusEnemySpawnChance * 100 + EnemySpawnChance;
		if (FMath::RandRange(1, 100) <= AdjustedChance)
		{
			SpawnEnemy(0.f, Enemy, 1);
		}
	}
}

void ADonGameModeBase::SpawnEnemy(float SpawnTime, TSubclassOf<ACharacter> Enemy, int32 Amount)
{
	if (!bStopLevel)
	{
		for (int32 Idx = 0; Idx < FMath::Min(Amount, MaxSpawnAmount); Idx++)
		{
			if (SpawnedEnemyAmount >= MaxEnemyEntities) break;
			
			FVector Center;
			APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
			if (PlayerPawn)	Center = PlayerPawn->GetActorLocation();

			float Radius = SpawnDistance;
			float Angle = FMath::RandRange(0.f, 2 * PI);
			float X = Radius * FMath::Cos(Angle);
			float Y = Radius * FMath::Sin(Angle);

			FVector SpawnLocation = Center + FVector(X, Y, 0.f);

			FActorSpawnParameters SpawnParameters;
			FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);
			
			ADonEnemy* EnemyActor = GetWorld()->SpawnActorDeferred<ADonEnemy>(Enemy, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			if (EnemyActor)
			{
				int32 Level = FMath::Max(1, 1 + FMath::FloorToInt32(GetWorld()->GetTimeSeconds() / EnemyUpgradeTime));
				UE_LOG(LogTemp, Warning, TEXT("Enemy Level : %d"), Level);
				EnemyActor->SetCharacterLevel(Level);
				UGameplayStatics::FinishSpawningActor(EnemyActor, SpawnTransform);
				AddToSpawnedEnemies(1);
				if (EnemyActor->bBossEnemy) break;
			}
		}
	}
	
	if (GetWorld()->GetTimeSeconds() < SpawnEndTime && SpawnTime > 0.f)
	{
		FTimerDelegate EnemyTimerDelegate;
		int32 AmountToAdd = FMath::FloorToInt(GetWorld()->GetTimeSeconds() / WaveTime);
		int32 NewSpawnAmount = FMath::RandRange(FMath::Max(1, Amount + AmountToAdd - 3), Amount + AmountToAdd + 1);
		EnemyTimerDelegate.BindUFunction(this, FName("SpawnEnemy"), SpawnTime, Enemy, NewSpawnAmount);

		FTimerHandle EnemyTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(EnemyTimerHandle, EnemyTimerDelegate, SpawnTime, false);
	}
}

void ADonGameModeBase::SpawnNPC(TSubclassOf<ACharacter> NPC)
{
	if (NPC == nullptr) return;
	
	if (ANPCCharacterBase* NPCActor = GetWorld()->SpawnActorDeferred<ANPCCharacterBase>(NPC, FTransform(FRotator(0.f, 135.f, 0.f))))
	{
		if (AMerchantNPC* Merchant = Cast<AMerchantNPC>(NPCActor))
		{
			Merchant->bLoadBonusMerchandise = false;
			Merchant->LifeTimer = NPCLifeTime;
		}
		UGameplayStatics::FinishSpawningActor(NPCActor, FTransform(FRotator(0.f, 135.f, 0.f)));
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADonEnemy::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ADonEnemy* BossEnemy = Cast<ADonEnemy>(Actor))
		{
			BossEnemy->DestroyStone();
		}
		Actor->Destroy();
		bStopLevel = true;
		FTimerHandle ResumeLevelTimer;
		FTimerDelegate ResumeLevelDelegate;
		ResumeLevelDelegate.BindLambda(
	[this]()
			{
				bStopLevel = false;
			}
		);
		GetWorld()->GetTimerManager().SetTimer(ResumeLevelTimer, ResumeLevelDelegate, NPCLifeTime, false);
	}
}
