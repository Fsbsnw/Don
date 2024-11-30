// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Dialogue.generated.h"

/**
 * Begin ENUM
 */

UENUM(BlueprintType)
enum class ENPCName : uint8
{
	Normal     UMETA(DisplayName = "Normal NPC"),
	Merchant      UMETA(DisplayName = "Merchant NPC")
};

UENUM(BlueprintType)
enum class EDialogueFlow : uint8
{
	Normal				UMETA(DisplayName = "Normal"),
	Branch				UMETA(DisplayName = "Branch")
};

UENUM(BlueprintType)
enum class EDialogueType : uint8
{
	Normal				UMETA(DisplayName = "Normal"),
	ContinueFromLast	UMETA(DisplayName = "ContinueFromLast"),
	ResponseChoice		UMETA(DisplayName = "Response Choice"),
	QuestOffer			UMETA(DisplayName = "Quest Offer"),
	End					UMETA(DisplayName = "End"),
	ChoicedResponse	UMETA(DisplayName = "Choiced Response")
};

UENUM(BlueprintType)
enum class EResponseType : uint8
{
	Choice     UMETA(DisplayName = "Choice"),
	Quest      UMETA(DisplayName = "Quest")
};

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	QuestComplete     UMETA(DisplayName = "Quest Complete"),
	HasItem           UMETA(DisplayName = "Has Item"),
	DialogueComplete  UMETA(DisplayName = "DialogueComplete ") 
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
struct FObjective
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EObjectiveType ObjectiveType; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle ObjectiveDataHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ItemAmount = 1;
};

USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase
{
	GENERATED_BODY()

	bool operator==(const FDialogue& Other) const
	{
		return	NPCName == Other.NPCName &&
				DialogueFlow == Other.DialogueFlow &&
				DialogueBranch == Other.DialogueBranch &&
				DialogueProgress == Other.DialogueProgress;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ENPCName NPCName = ENPCName::Normal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDialogueFlow DialogueFlow = EDialogueFlow::Normal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDialogueType DialogueType = EDialogueType::End;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DialogueBranch = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DialogueProgress = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DialogueText;

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