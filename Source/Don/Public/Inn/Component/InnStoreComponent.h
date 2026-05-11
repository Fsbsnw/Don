// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemStructs.h"
#include "Player/Interface/SaveableInterface.h"
#include "InnStoreComponent.generated.h"

struct FInnUpgradeResources;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInnMerchandiseChanged, const TArray<FItem>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpgradeResourcesChanged, const FInnUpgradeResources&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpgradeRequested, bool);
DECLARE_MULTICAST_DELEGATE(FOnDevilPotionPurchased);

USTRUCT(BlueprintType)
struct FMerchandiseData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemAmount;
};

USTRUCT(BlueprintType)
struct FInnUpgradeResources
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 RequiredLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 RequiredCost;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 RequiredCrystal;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInnStoreComponent : public UActorComponent, public ISaveableInterface
{
	GENERATED_BODY()

public:	
	UInnStoreComponent();

	virtual void SavePlayerData(FPlayerSaveData& Data) override;
	virtual void LoadPlayerData(const FPlayerSaveData& InData) override;

	UPROPERTY(EditDefaultsOnly)
	TArray<FMerchandiseData> InitialMerchandise;
	TArray<FItem> InnMerchandise;

	void InitializeStoreMerchandise();
	bool SellMerchandise(int32 ItemIndex);
	bool UpgradeInn(bool bTargetIsTable);

	FOnUpgradeRequested OnUpgradeRequested;
	FOnDevilPotionPurchased OnDevilPotionPurchased;
	
	int32 InnUpgradeLevel = 1;
	int32 InnUpgradeCost = 2500;
	int32 InnUpgradeCrystal = 5;

	int32 HiringChefCount = 1;
	int32 ChefHiringCost = 5000;
	int32 ChefHiringCrystal = 10;

	FInnUpgradeResources GetTableUpgradeResources();
	FInnUpgradeResources GetChefUpgradeResources();

	FOnUpgradeResourcesChanged OnTableUpgradeChanged;
	FOnUpgradeResourcesChanged OnChefUpgradeChanged;
	FOnInnMerchandiseChanged OnInnMerchandiseChanged;
};