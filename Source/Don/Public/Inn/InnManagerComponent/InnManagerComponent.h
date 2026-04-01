// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/Interface/SaveableInterface.h"
#include "InnManagerComponent.generated.h"

class AInnChef;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInnChanged, int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInnManagerComponent : public UActorComponent, public ISaveableInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	UInnManagerComponent();

	virtual void SavePlayerData(FPlayerSaveData& Data) override;
	virtual void LoadPlayerData(const FPlayerSaveData& InData) override;

	FORCEINLINE int32 GetReputation() const { return Reputation; }
	FORCEINLINE int32 GetPopularity() const { return Popularity; }
	FORCEINLINE int32 GetInterior() const { return Interior; }
	FORCEINLINE int32 GetSuspicion() const { return Suspicion; }
	FORCEINLINE int32 GetInnLevel() const { return InnLevel; }

	void AddToReputation(int32 InReputation);
	void AddToPopularity(int32 InPopularity);
	void AddToInterior(int32 InInterior);
	void AddToSuspicion(int32 InSuspicion);
	void AddToInnLevel(int32 InInnLevel);

	FOnInnChanged OnReputationChanged;
	FOnInnChanged OnPopularityChanged;
	FOnInnChanged OnInteriorChanged;
	FOnInnChanged OnSuspicionChanged;
	FOnInnChanged OnInnLevelChanged;

	void HandleUpgradeRequest(bool bTargetIsTable);
	bool HireNewChef();
	void UpgradeInn();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AInnChef>> ChefsClasses;
	
	UPROPERTY()
	AInnChef* Chefs[3];

	UPROPERTY()
	FInnChefSaveData ChefSaveData[3];
	
private:
	int32 Reputation = 0;
	int32 Popularity = 0;
	int32 Interior = 0;
	int32 Suspicion = 0;
	int32 InnLevel = 1;
};
