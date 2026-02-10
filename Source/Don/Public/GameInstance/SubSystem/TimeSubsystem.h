// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "TimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGameTimeChanged, int32, Day, int32, Hour, int32, Minute);
DECLARE_MULTICAST_DELEGATE(FOnMorning);

/**
 * 
 */
UCLASS()
class DON_API UTimeSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override { return true; }
	UFUNCTION(BlueprintCallable)
	void BroadcastInitialTime();

	UPROPERTY(BlueprintAssignable)
	FOnGameTimeChanged OnGameTimeChanged;

	FOnMorning OnMorning;
	
protected:
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetCurrentTime() const { return CurrentTime; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetCurrentDay() const { return FMath::FloorToInt(CurrentTime) / (60 * 24) + 1; }
	
	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetCurrentHour() const { return (FMath::FloorToInt(CurrentTime) / 60) % 24; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetCurrentMinute() const { return FMath::FloorToInt(CurrentTime) % 60 / 10; }
	
	
private:
	float CurrentTime = 0.f;
};
