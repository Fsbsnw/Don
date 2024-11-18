// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Dialogue.h"
#include "Engine/DataTable.h"
#include "Quest.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EQuestState
{
	NotStarted	UMETA(DisplayName = "Not Started"),
	InProgress	UMETA(DisplayName = "In Progress"),
	Completed	UMETA(DisplayName = "Completed")
};

UENUM(BlueprintType)
enum class EQuestReward
{
	XP		UMETA(DisplayName = "XP"),
	Item	UMETA(DisplayName = "Item")
};

USTRUCT(BlueprintType)
struct FQuestReward
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EQuestReward RewardType = EQuestReward::XP;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 XPAmount;
};

USTRUCT(BlueprintType)
struct FQuest
{
	GENERATED_BODY()

	bool operator==(const FQuest& Other) const
	{
		return Other.QuestNPC == this->QuestNPC && Other.QuestID == this->QuestID;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString QuestNPC;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName QuestID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EQuestState QuestState = EQuestState::NotStarted;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString QuestTitle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString QuestDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCondition> QuestObjectives;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FQuestReward> QuestRewards;
};

USTRUCT(BlueprintType)
struct FQuestRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString QuestNPC;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FQuest> Quests;
};

USTRUCT(BlueprintType)
struct FQuestContainer
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FQuest> Quests;
};