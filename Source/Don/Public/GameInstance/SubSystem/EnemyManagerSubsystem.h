// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemyManagerSubsystem.generated.h"

class ADonEnemyPawn;

enum EAILODLevel
{
	
};

/**
 * 
 */
UCLASS()
class DON_API UEnemyManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void UpdateAILOD();
	
	TArray<ADonEnemyPawn*> EnemyPawns;
	FTimerHandle UpdateAIsTimerHandle;

	int32 NextEnemyToUpdateIndex = 0;
	int32 MaxEnemyCount = 0;
	int32 MaxEnemiesPerFrame = 10;
	int32 TestCountBT = 0;
	int32 TestCountNotBT = 0;

public:
	void RegisterEnemyPawn(ADonEnemyPawn* EnemyPawn);
};
