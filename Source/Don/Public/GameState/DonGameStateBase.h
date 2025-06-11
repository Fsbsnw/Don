// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DonGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class DON_API ADonGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	float GetPlayTime() const { return PlayTime; }
	UFUNCTION(BlueprintCallable)
	void SetPlayTimerPaused(bool bPaused) { bIsTimerPaused = bPaused; }
	UFUNCTION(BlueprintCallable)
	void ProgressPlayTime(float DeltaTime) { if (bIsTimerPaused) PlayTime += DeltaTime; }
	
private:
	float PlayTime = 0.f;
	bool bIsTimerPaused = false;
};
