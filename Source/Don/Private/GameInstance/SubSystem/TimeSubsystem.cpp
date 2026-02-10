// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SubSystem/TimeSubsystem.h"

void UTimeSubsystem::Tick(float DeltaTime)
{
	int32 PrevMinute = GetCurrentMinute();
	CurrentTime += DeltaTime;
	int32 CurrMinute = GetCurrentMinute();

	if (PrevMinute != CurrMinute)
	{
		OnGameTimeChanged.Broadcast(GetCurrentDay(), GetCurrentHour(), GetCurrentMinute());
		
		if (CurrMinute == 3)
		{
			OnMorning.Broadcast();
		}
	}
}

TStatId UTimeSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTimeSubsystem, STATGROUP_Tickables);
}

void UTimeSubsystem::BroadcastInitialTime()
{
	OnGameTimeChanged.Broadcast(GetCurrentDay(), GetCurrentHour(), GetCurrentMinute());
}
