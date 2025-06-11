// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "QuestListWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillCountSignature, int32, KillCount);
struct FQuest;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DON_API UQuestListWidgetController : public UDonWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies() override;
	UPROPERTY(BlueprintAssignable)
	FOnKillCountSignature OnKillCountDelegate;
	
	UFUNCTION(BlueprintCallable)
	void AwardQuestRewards(FString QuestTitle);

	UFUNCTION(BlueprintCallable)
	FQuest FindPlayerQuest(FString QuestTitle);

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerKillCount();
};

