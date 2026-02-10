// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DonInnGameMode.generated.h"

struct FRoomInfo;
/**
 * 
 */
UCLASS()
class DON_API ADonInnGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddToRevenue(int32 InMoney);

	UFUNCTION(BlueprintCallable)
	void AddToReputation(int32 InReputation);

	UFUNCTION(BlueprintCallable)
	void AddToPopularity(int32 InPopularity);
	
	UFUNCTION(BlueprintCallable)
	void AddToInterior(int32 InInterior);

	int32 GetReputation();
	int32 GetPopularity();
	int32 GetInterior();

	UPROPERTY(EditDefaultsOnly)
	TArray<FRoomInfo> RoomInfos;
};
