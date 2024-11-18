// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Dialogue.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FResponseOption
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Response;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle NextDialogueHandle;
};

UENUM(BlueprintType)
enum class EConditionType : uint8
{
	QuestComplete     UMETA(DisplayName = "Quest Complete"),
	HasItem           UMETA(DisplayName = "Has Item"),
	DialogueComplete  UMETA(DisplayName = "DialogueComplete ") 
};

USTRUCT(BlueprintType)
struct FCondition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EConditionType ConditionType; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle ConditionDataHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName;
};

UENUM(BlueprintType)
enum class EDialogueType : uint8
{
	Normal				UMETA(DisplayName = "Normal"),
	ContinueFromLast	UMETA(DisplayName = "ContinueFromLast"),
	ResponseChoice		UMETA(DisplayName = "Response Choice"),
	QuestOffer			UMETA(DisplayName = "Quest Offer"),
	End					UMETA(DisplayName = "End"),
	SelectedResponse	UMETA(DisplayName = "Selected Response")
};

USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase
{
	GENERATED_BODY()

	bool operator==(const FDialogue& Other) const
	{
		return DialogueID == Other.DialogueID && DialogueProgress == Other.DialogueProgress;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString DialogueID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDialogueType DialogueType = EDialogueType::End;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DialogueProgress = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DialogueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FResponseOption> ResponseOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCondition> Conditions;
};

USTRUCT(BlueprintType)
struct FDialogueContainer
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FDialogue> Dialogues;
};