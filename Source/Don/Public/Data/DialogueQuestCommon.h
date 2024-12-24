#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "DialogueQuestCommon.generated.h"

UENUM(BlueprintType)
enum class ENPCName : uint8
{
	Normal     UMETA(DisplayName = "Normal NPC"),
	Merchant      UMETA(DisplayName = "Merchant NPC")
};

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	QuestComplete     UMETA(DisplayName = "Quest Complete"),
	HasItem           UMETA(DisplayName = "Has Item"),
	DialogueComplete  UMETA(DisplayName = "DialogueComplete ") 
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