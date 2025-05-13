// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AbilityTasks/AbilityTask_Timeline.h"

#include "TimerManager.h"

UAbilityTask_Timeline::UAbilityTask_Timeline(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

UAbilityTask_Timeline* UAbilityTask_Timeline::AbilityTaskOnTick(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UAbilityTask_Timeline* MyObj = NewAbilityTask<UAbilityTask_Timeline>(OwningAbility);
	return MyObj;
}

void UAbilityTask_Timeline::Activate()
{
	Super::Activate();
	
	SetWaitingOnAvatar();
}

void UAbilityTask_Timeline::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		Value += DeltaTime * Multiplier;
		OnTick.Broadcast(Value);
		if (Value > 1.f)
		{
			EndTask();
		}
	}
}
