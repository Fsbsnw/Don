// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SubSystem/TimeSubsystem.h"

#include "GameInstance/DonGameInstance.h"


void UTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UDonGameInstance* DGI = Cast<UDonGameInstance>(GetGameInstance());
	if (DGI)
	{
		TimeMultiplier = DGI->SubSystemTimeMultiplier;
	}
}

void UTimeSubsystem::Tick(float DeltaTime)
{
	if (bIsPaused || FMath::IsNearlyZero(TimeMultiplier)) return;
	
	int32 PrevHour = GetCurrentHour();
	int32 PrevTenMinute = GetCurrentTenMinuteUnit();
	CurrentTime += GetWorld()->GetDeltaSeconds() * TimeMultiplier;
	int32 CurrHour = GetCurrentHour();
	int32 CurrTenMinute = GetCurrentTenMinuteUnit();

	if (PrevTenMinute != CurrTenMinute)
	{
		OnGameTimeChanged.Broadcast(GetCurrentDay(), GetCurrentHour(), GetCurrentTenMinuteUnit());

		if (PrevHour != CurrHour)
		{
			// Daybreak Check
			if (CurrHour == 6)
			{
				OnDaybreak.Broadcast();
				PauseTime();
			}
			
			// Morning Check
			if (CurrHour == 9)
			{
				OnMorning.Broadcast();
			}
			
			// Midnight Check
			if (CurrHour == 0)
			{
				OnMidnight.Broadcast();
				PauseTime();
			}
		}
	}
}

TStatId UTimeSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTimeSubsystem, STATGROUP_Tickables);
}

void UTimeSubsystem::BroadcastInitialTime()
{
	OnGameTimeChanged.Broadcast(GetCurrentDay(), GetCurrentHour(), GetCurrentTenMinuteUnit());
}

bool UTimeSubsystem::SkipToMidnight()
{
	if (GetCurrentHour() < 6) return false;
	
	float CurrentMinutes = CurrentTime;
	float MinutesToMidnight = 1440.0f - FMath::Fmod(CurrentMinutes, 1440.0f);

	// 정확히 자정에 이미 있다면 스킵 안 하도록
	if (MinutesToMidnight <= KINDA_SMALL_NUMBER)
	{
		return false;
	}

	if (GetCurrentHour() < 9)
	{
		OnMorning.Broadcast();
	}
	CurrentTime += MinutesToMidnight;

	int32 CurrHour = GetCurrentHour();
	int32 CurrTenMinute = GetCurrentTenMinuteUnit();

	OnGameTimeChanged.Broadcast(GetCurrentDay(), CurrHour, CurrTenMinute);

	OnMidnight.Broadcast();
	PauseTime();
	return true;
}

bool UTimeSubsystem::SkipToDaybreak()
{
	if (GetCurrentHour() >= 6) return false;

	float CurrentMinutesInDay = FMath::Fmod(CurrentTime, 1440.0f);

	float TargetMinutes = 360.0f; // 06:00 = 6 * 60
	float MinutesToAdd = 0.0f;

	if (CurrentMinutesInDay < TargetMinutes)
	{
		// 00:00 ~ 05:59 → 바로 06:00로
		MinutesToAdd = TargetMinutes - CurrentMinutesInDay;
	}

	if (MinutesToAdd <= KINDA_SMALL_NUMBER)
	{
		return false;
	}
	
	CurrentTime += MinutesToAdd;

	OnGameTimeChanged.Broadcast(GetCurrentDay(), GetCurrentHour(), GetCurrentTenMinuteUnit());

	OnDaybreak.Broadcast();
	PauseTime();
	return true;
}

void UTimeSubsystem::ResetSystem()
{
	CurrentTime = 360.f;
	OnMorning.Clear();
}