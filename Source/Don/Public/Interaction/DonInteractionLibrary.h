// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DonInteractionLibrary.generated.h"

enum class ENPCName : uint8;
struct FDonDialogueContext;
struct FDialogue;
struct FQuest;
class UQuestListWidgetController;
/**
 * 
 */
UCLASS()
class DON_API UDonInteractionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Quest")
	static bool FindQuestRow(const UObject* WorldContextObject, FQuest& OutQuest, ENPCName NPCName, FString QuestTitle);

	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Dialogue")
	static bool FindDialogueRow(const UObject* WorldContextObject, FDialogue& OutDialogue, FDonDialogueContext& DialogueContext);

	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Dialogue")
	static UTexture2D* GetNPCImage(const UObject* WorldContextObject, ENPCName NPCName);
};
