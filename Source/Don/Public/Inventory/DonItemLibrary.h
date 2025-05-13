// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Actor/DonEquipmentActor.h"
#include "Character/DonCharacterBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/Dialogue.h"
#include "Data/ItemAsset.h"
#include "Data/Quest.h"
#include "DonItemLibrary.generated.h"

class UAbilityInfo;
class UAttributeMenuWidgetController;
struct FDamageEffectParams;
enum class ECharacterClass;
struct FGameplayTag;
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

	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Attribute")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Quest")
	static UQuestListWidgetController* GetQuestListWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static FCharacterClassInfo FindCharacterClassInfo(const UObject* WorldContextObject, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Equipment")
	static FEquipmentAttributes FindEquipmentAttributes(const UObject* WorldContextObject, FName EquipmentName);

	UFUNCTION(BlueprintCallable, Category = "DonItemLibrary | Item")
	static void SpawnLootableXP(const UObject* WorldContextObject, int32 Amount, FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION(BlueprintCallable, Category = "DonItemLibrary | Item")
	static void SpawnLootableMoney(const UObject* WorldContextObject, int32 Amount, int32 Quantity, FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION(BlueprintCallable, Category = "DonItemLibrary | Item")
	static void SpawnLootableItem(const UObject* WorldContextObject, FName ItemName, FVector SpawnLocation, FRotator SpawnRotation);
	
	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Quest")
	static bool FindQuestRow(const UObject* WorldContextObject, FQuest& OutQuest, ENPCName NPCName, FString QuestTitle);

	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Dialogue")
	static bool FindDialogueRow(const UObject* WorldContextObject, FDialogue& OutDialogue, FDonDialogueContext& DialogueContext);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
};