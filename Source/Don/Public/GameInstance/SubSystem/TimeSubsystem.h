// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "TimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGameTimeChanged, int32, Day, int32, Hour, int32, Minute);
DECLARE_MULTICAST_DELEGATE(FOnTimePeriodChanged);

/**
 * 
 */
UCLASS()
class DON_API UTimeSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override { return true; }
	UFUNCTION(BlueprintCallable)
	void BroadcastInitialTime();

	UPROPERTY(BlueprintAssignable)
	FOnGameTimeChanged OnGameTimeChanged;

	FOnTimePeriodChanged OnMorning;
	FOnTimePeriodChanged OnMidnight;
	FOnTimePeriodChanged OnDaybreak;

	UFUNCTION(BlueprintCallable)
	bool SkipToMidnight();

	UFUNCTION(BlueprintCallable)
	bool SkipToDaybreak();

	UFUNCTION(BlueprintCallable)
	void PauseTime() { bIsPaused = true; };
	UFUNCTION(BlueprintCallable)
	void ResumeTime() { bIsPaused = false; };
	UFUNCTION(BlueprintCallable)
	void ResetSystem();
	
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetCurrentTime() const { return CurrentTime; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetCurrentDay() const { return FMath::FloorToInt(CurrentTime) / (60 * 24) + 1; }
	
	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetCurrentHour() const { return (FMath::FloorToInt(CurrentTime) / 60) % 24; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetCurrentTenMinuteUnit() const { return FMath::FloorToInt(CurrentTime) % 60 / 10; }
	
	
private:
	bool bIsPaused = true;
	float CurrentTime = 360.f;
	float TimeMultiplier = 0.f;
};
