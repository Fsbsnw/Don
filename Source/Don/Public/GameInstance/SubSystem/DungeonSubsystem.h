// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DungeonSubsystem.generated.h"

class APatrolPath;
class UInnCustomerGroup;
/**
 * 
 */
UCLASS()
class DON_API UDungeonSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeData();

	UFUNCTION(BlueprintCallable)
	void SpawnLodgerEnemy();

	UFUNCTION()
	void CalculateDeadMembers(int32 GroupID);
	
	UPROPERTY()
	TArray<UInnCustomerGroup*> LodgerGroups;

	UPROPERTY()
	TArray<APatrolPath*> Paths;
	
	int32 PathIndex = 0;
	int32 LevelEnemyCount = 0;
	int32 LevelEnemyKillCount = 0;
	bool bInitialized = false;
};
