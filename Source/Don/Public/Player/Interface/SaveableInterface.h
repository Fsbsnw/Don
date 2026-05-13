// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/ItemStructs.h"
#include "SaveableInterface.generated.h"


USTRUCT()
struct FInnChefSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 ChefLevel = 1;

	UPROPERTY(EditAnywhere)
	int32 ChefXP = 0;
	
	UPROPERTY(EditAnywhere)
	bool bIsHired = false;
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere)
	int32 XP = 0;
	
	UPROPERTY(EditAnywhere)
	int32 AttributePoints = 10;
	
	UPROPERTY(EditAnywhere)
	int32 SkillPoints = 0;

	UPROPERTY(EditAnywhere)
	int32 Money = 0;

	UPROPERTY(EditAnywhere)
	int32 AxeUpgrade = 0;

	UPROPERTY(EditAnywhere)
	int32 GameScore = 0;

	// Inn Manager Component
	
	UPROPERTY(EditAnywhere)
	int32 InnLevel = 1;
	
	UPROPERTY(EditAnywhere)
	int32 HiringChefCount = 1;
	
	UPROPERTY(EditAnywhere)
	int32 ChefHiringCost = 5000;
	
	UPROPERTY(EditAnywhere)
	int32 ChefHiringCrystal = 15;
	
	UPROPERTY(EditAnywhere)
	int32 Reputation = 0;
	
	UPROPERTY(EditAnywhere)
	int32 Popularity = 0;
	
	UPROPERTY(EditAnywhere)
	int32 Interior = 0;
	
	UPROPERTY(EditAnywhere)
	int32 Suspicion = 0;


	// Inventory Component

	UPROPERTY()
	TArray<FItem> Inventory;

	// Inn Store Component

	UPROPERTY(EditAnywhere)
	int32 InnUpgradeCost = 4000;

	UPROPERTY(EditAnywhere)
	int32 InnUpgradeCrystal = 10;

	UPROPERTY(EditAnywhere)
	TArray<FItem> InnMerchandise;

	// Inn Chef

	UPROPERTY(EditAnywhere)
	FInnChefSaveData ChefData[3];
};

UINTERFACE(MinimalAPI)
class USaveableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DON_API ISaveableInterface
{
	GENERATED_BODY()

public:
	virtual void SavePlayerData(FPlayerSaveData& Data) = 0;
	virtual void LoadPlayerData(const FPlayerSaveData& InData) = 0;
};
