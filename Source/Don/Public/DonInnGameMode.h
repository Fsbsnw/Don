// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DonInnGameMode.generated.h"

struct FCompletedFoodOrder;
struct FRoomInfo;
/**
 * 
 */
UCLASS()
class DON_API ADonInnGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
	void CloseInnSystem(int32& OutRoomServiceRevenue);
	void CloseInnKitchen(TArray<FCompletedFoodOrder>& OutCompletedOrder);
	void HandleMidnight();
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void EnterMidnightForUI(const TArray<FCompletedFoodOrder>& CompletedOrder, int32 RoomServiceRevenue);

	UFUNCTION(BlueprintCallable)
	void EnterDungeon();
	
	UFUNCTION(BlueprintCallable)
	void AddToRevenue(int32 InMoney);

	UFUNCTION(BlueprintCallable)
	void AddToReputation(int32 InReputation);

	UFUNCTION(BlueprintCallable)
	void AddToPopularity(int32 InPopularity);
	
	UFUNCTION(BlueprintCallable)
	void AddToInterior(int32 InInterior);
	
	UFUNCTION(BlueprintCallable)
	void AddToInnLevel(int32 InInnLevel);

	UFUNCTION(BlueprintCallable)
	void AddToSuspicion(int32 InSuspicion);

	UFUNCTION(BlueprintImplementableEvent)
	void GameOverForUI(int32 Type);
	void GameOver(int32 Type);

	int32 GetReputation();
	int32 GetPopularity();
	int32 GetInterior();

	UPROPERTY(EditAnywhere)
	int32 Day;
	UPROPERTY(EditAnywhere)
	int32 Hour;
	UPROPERTY(EditAnywhere)
	int32 Minute;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FRoomInfo> RoomInfos;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DungeonLevel;
};