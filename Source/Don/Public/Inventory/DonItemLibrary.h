// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/Dialogue.h"
#include "Data/ItemAsset.h"
#include "Data/Quest.h"
#include "DonItemLibrary.generated.h"

class UInventoryWidgetController;
class UQuestListWidgetController;
/**
 * 
 */
UCLASS()
class DON_API UDonItemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static FItem FindItemByName(const UObject* WorldContextObject, FName ItemName);

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static UInventoryWidgetController* GetInventoryWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Quest")
	static UQuestListWidgetController* GetQuestListWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Quest")
	static bool FindQuestRow(const UObject* WorldContextObject, FQuest& OutQuest, ENPCName NPCName, FString QuestTitle);

	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Dialogue")
	static bool FindDialogueRow(const UObject* WorldContextObject, FDialogue& OutDialogue, FDonDialogueContext& DialogueContext);
};
