#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "DonCharacterTypes.generated.h"


UENUM(BlueprintType)
enum class ECharacterClass
{
	Fighter		UMETA(DisplayName = "Fighter"),
	Warrior		UMETA(DisplayName = "Warrior"),
	Ranger		UMETA(DisplayName = "Ranger")
};

USTRUCT(BlueprintType)
struct FCharacterClassInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterClass CharacterClass = ECharacterClass::Fighter;
};


UENUM(BlueprintType)
enum class EEnemyClass
{
	Fighter		UMETA(DisplayName = "Fighter"),
	Warrior		UMETA(DisplayName = "Warrior"),
	Ranger		UMETA(DisplayName = "Ranger"),
	Boss		UMETA(DisplayName = "Boss")
};

USTRUCT(BlueprintType)
struct FEnemyClassInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEnemyClass EnemyClass = EEnemyClass::Fighter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DroppableMoney = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DroppableXP = 0;
};