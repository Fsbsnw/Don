// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/Dialogue.h"
#include "Data/ItemAsset.h"
#include "DonItemLibrary.generated.h"

struct FDialogueContainer;
struct FDialogue;
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

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Quest")
	static UQuestListWidgetController* GetQuestListWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Dialogue")
	static bool FindDialogueRow(const UObject* WorldContextObject, FDialogue& OutDialogue , const ENPCName& NPCName, const EDialogueFlow& Flow, int32 Branch = 0, int32 Progress = 1);
};
