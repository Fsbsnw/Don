// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DonGameModeBase.generated.h"

class ADonEnemy;
class UNPCInfo;
class ULootableActorDataAsset;
class UItemAsset;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Information")
	TObjectPtr<UItemAsset> ItemInfoAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC Information")
	TObjectPtr<UNPCInfo> NPCInfoAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lootable Actor Information")
	TObjectPtr<ULootableActorDataAsset> LootableActorAsset;

	UPROPERTY(EditDefaultsOnly)
	TMap<float, TSubclassOf<ADonEnemy>> BossSpawnTimer;

	UFUNCTION()
	void SpawnBoss(float SpawnTime, TSubclassOf<ADonEnemy> Boss);
};
