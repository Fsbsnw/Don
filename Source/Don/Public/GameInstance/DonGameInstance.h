// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DonEquipmentActor.h"
#include "Engine/GameInstance.h"
#include "Data/Dialogue.h"
#include "Data/Quest.h"
#include "DonGameInstance.generated.h"

class UAbilityInfo;
class UDataTable;
/**
 * 
 */
UCLASS()
class DON_API UDonGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	bool InitDialogueDataTable();
	bool InitQuestDataTable();
	bool InitEquipmentDataTable();
	virtual void Init() override;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<ENPCName, FDialogueContainer> DialogueDataTable;

	UPROPERTY(BlueprintReadOnly)
	TMap<ENPCName, FQuestContainer> QuestDataTable;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FEquipmentAttributes> EquipmentDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* CharacterClassDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAbilityInfo* AbilityInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* MerchandiseDataTable;
};
