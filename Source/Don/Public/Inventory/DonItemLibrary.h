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

	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Dialogue")
	static bool FindDialogueRow(const UObject* WorldContextObject, UPARAM(ref) FDialogue& OutDialogue , const FString& NPCName, int32 Progress = 1);

	UFUNCTION(BlueprintCallable, Category = "DonInteractLibrary | Dialogue")
	static void AddDialogueRow(UPARAM(ref) TMap<FString, FDialogueContainer>& DialogueContainer, const FDialogue& Dialogue);
};
