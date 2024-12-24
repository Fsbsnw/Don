// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Data/DialogueQuestCommon.h"
#include "Engine/DataTable.h"
#include "Dialogue.generated.h"

/**
 * Begin ENUM
 */

UENUM(BlueprintType)
enum class EDialogueType : uint8
{
	Normal				UMETA(DisplayName = "Normal"),
	ContinueFromLast	UMETA(DisplayName = "ContinueFromLast"),
	ResponseChoice		UMETA(DisplayName = "Response Choice"),
	QuestOffer			UMETA(DisplayName = "Quest Offer"),
	End					UMETA(DisplayName = "End"),
	ChoicedResponse		UMETA(DisplayName = "Choiced Response"),
	QuestReward			UMETA(DisplayName = "Quest Reward")
};

UENUM(BlueprintType)
enum class EResponseType : uint8
{
	Choice     UMETA(DisplayName = "Choice"),
	Quest      UMETA(DisplayName = "Quest")
};

/*
 * End ENUM
 */



/*
 * Start STRUCT
 */

USTRUCT(BlueprintType)
struct FResponseOption
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EResponseType ResponseType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Response;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle NextDialogueHandle;
};

USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase
{
	GENERATED_BODY()

	FDialogue(){}
	FDialogue(ENPCName InNPCName)
		: NPCName(InNPCName)
	{}
	
	bool operator==(const FDialogue& Other) const
	{
		return	this->NPCName == Other.NPCName &&
				this->Chapter == Other.Chapter &&
				this->Topic == Other.Topic &&
				this->Branch == Other.Branch &&
				this->Progress == Other.Progress;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ENPCName NPCName = ENPCName::Normal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Chapter = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Topic;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Branch = "Default";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Progress = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDialogueType Type = EDialogueType::End;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Text;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FResponseOption> ResponseOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FObjective> Objectives;
};

USTRUCT(BlueprintType)
struct FDialogueContainer
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FDialogue> Dialogues;
};

USTRUCT(BlueprintType)
struct FDonDialogueContext
{
	GENERATED_BODY()

	FDonDialogueContext()
	{
	}

	FDonDialogueContext(ENPCName InNPCName, int32 InChapter, FString InTopic, FString InBranch, int32 InProgress)
		: NPCName(InNPCName), Chapter(InChapter), Topic(InTopic), Branch(InBranch), Progress(InProgress)
	{
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ENPCName NPCName = ENPCName::Normal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Chapter = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Topic;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Branch = "Normal";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Progress = 1;
};