// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Data/DialogueQuestCommon.h"
#include "Engine/DataTable.h"
#include "Quest.generated.h"

/**
 * Begin ENUM
 */

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	MainQuest	UMETA(DisplayName = "Main Quest"),
	SideQuest	UMETA(DisplayName = "Side Quest")
};

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	NotStarted	UMETA(DisplayName = "Not Started"),
	InProgress	UMETA(DisplayName = "In Progress"),
	Completed	UMETA(DisplayName = "Completed")
};

UENUM(BlueprintType)
enum class EQuestRewardType : uint8
{
	XP		UMETA(DisplayName = "XP"),
	Item	UMETA(DisplayName = "Item")
};

/*
 * End ENUM
 */

/*
 * Begin STRUCT
 */

USTRUCT(BlueprintType)
struct FQuestReward
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EQuestRewardType RewardType = EQuestRewardType::XP;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount;
};

USTRUCT(BlueprintType)
struct FQuest
{
	GENERATED_BODY()

	FQuest(){}
	FQuest(const ENPCName InNPCName, const FString& InTitle)
		: QuestNPC(InNPCName), QuestTitle(InTitle){}

	bool operator==(const FQuest& Other) const
	{
		return Other.QuestNPC == this->QuestNPC && Other.QuestTitle == this->QuestTitle;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ENPCName QuestNPC = ENPCName::Normal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EQuestState QuestState = EQuestState::NotStarted;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString QuestTitle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString QuestDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FObjective> QuestObjectives;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FQuestReward> QuestRewards;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle DialogueOnQuestCompletion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EQuestType QuestType = EQuestType::MainQuest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle DialogueAfterQuestCompletion;
};

USTRUCT(BlueprintType)
struct FQuestRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ENPCName QuestNPC = ENPCName::Normal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FQuest Quest;
};

USTRUCT(BlueprintType)
struct FQuestContainer
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FQuest> Quests;
};