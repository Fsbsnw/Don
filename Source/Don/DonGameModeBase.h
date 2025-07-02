// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DonGameModeBase.generated.h"

class UItemConsumableAsset;
class UItemEquipmentAsset;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnedAmountSignature, int32, Amount);

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
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnSpawnedAmountSignature OnSpawnedAmountDelegate;
	
	int32 SpawnedEnemyAmount = 0;

	void AddToSpawnedEnemies(int32 Value);
	
	UPROPERTY(EditAnywhere)
	bool bStartLevel = true;
	bool bStopLevel = false;

	// Data Asset
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Information")
	TObjectPtr<UItemAsset> ItemInfoAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Information")
	TObjectPtr<UItemEquipmentAsset> ItemEquipmentAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Information")
	TObjectPtr<UItemConsumableAsset> ItemConsumableAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC Information")
	TObjectPtr<UNPCInfo> NPCInfoAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lootable Actor Information")
	TObjectPtr<ULootableActorDataAsset> LootableActorAsset;

	// End Data Asset
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FEnemySpawnInfo> EnemySpawnSchedule;

	UPROPERTY(EditDefaultsOnly)
	float SpawnDistance = 1000.f;

	UPROPERTY(EditDefaultsOnly)
	float NPCLifeTime = 60.f;
	
	UPROPERTY(EditDefaultsOnly)
	float EnemyUpgradeTime = 60.f;

	UPROPERTY(EditDefaultsOnly)
	float SpawnEndTime = 60.f * 15.f;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxSpawnAmount = 10;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxEnemyEntities = 50;

	UPROPERTY(EditDefaultsOnly)
	int32 WaveTime = 15.f;

	UPROPERTY(EditDefaultsOnly)
	float BonusEnemySpawnTime = 3.f;

	UPROPERTY(EditDefaultsOnly)
	float BonusEnemySpawnChance = 0.05f;
	
	UFUNCTION()
	void SpawnEnemy(float SpawnTime, TSubclassOf<ACharacter> Enemy, int32 Amount);
	UFUNCTION()
	void SpawnNPC(TSubclassOf<ACharacter> NPC);
	UFUNCTION()
	void SpawnBonusEnemy(TSubclassOf<ACharacter> Enemy);
};
