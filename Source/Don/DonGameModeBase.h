// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DonGameModeBase.generated.h"


class UNPCInfo;
class ULootableActorDataAsset;
class UItemAsset;


USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacter> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};

/**
 * 
 */
UCLASS()
class DON_API ADonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	UPROPERTY(EditAnywhere)
	bool bStartLevel = true;
	bool bStopLevel = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Information")
	TObjectPtr<UItemAsset> ItemInfoAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC Information")
	TObjectPtr<UNPCInfo> NPCInfoAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lootable Actor Information")
	TObjectPtr<ULootableActorDataAsset> LootableActorAsset;

	UPROPERTY(EditDefaultsOnly)
	TArray<FEnemySpawnInfo> EnemySpawnTimer;

	UPROPERTY(EditDefaultsOnly)
	float SpawnDistance = 1000.f;

	UPROPERTY(EditDefaultsOnly)
	float DestroyNPCTimer = 60.f;
	
	UPROPERTY(EditDefaultsOnly)
	float EnemyUpgradeTimer = 60.f;

	UPROPERTY(EditDefaultsOnly)
	float SpawnEndTimer = 60.f * 15.f;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxSpawnAmount = 50;

	UPROPERTY(EditDefaultsOnly)
	int32 SpawnWaveTimer = 15.f;

	UPROPERTY(EditDefaultsOnly)
	float BonusEnemySpawnTimer = 3.f;

	UPROPERTY(EditDefaultsOnly)
	float BonusEnemySpawnChance = 0.05f;
	
	UFUNCTION()
	void SpawnEnemy(float SpawnTime, TSubclassOf<ACharacter> Enemy, int32 Amount);
	UFUNCTION()
	void SpawnNPC(TSubclassOf<ACharacter> NPC);
	UFUNCTION()
	void SpawnBonusEnemy(TSubclassOf<ACharacter> Enemy);
};
