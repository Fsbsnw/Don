// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_Timeline.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimelineUpdateDelegate, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimelineFinishedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickTaskDelegate, float, DeltaTime);

UCLASS()
class DON_API UAbilityTask_Timeline : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnTickTaskDelegate OnTick;

	UAbilityTask_Timeline(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_Timeline* AbilityTaskOnTick(UGameplayAbility* OwningAbility, FName TaskInstanceName);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	float Value = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Multiplier = 1.f;

private:
	float TaskDeltaTime;
};