// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Dialogue.h"
#include "Data/Quest.h"
#include "InteractWidgetController.generated.h"

struct FDataTableRowHandle;
class UInteractComponent;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DON_API UInteractWidgetController : public UObject
{
	GENERATED_BODY()
public:	
	void SetWidgetControllerParams(UInteractComponent* InInteractComponent, APlayerState* TargetPlayerState);

	UFUNCTION(BlueprintCallable)
	void ResetInteractWidgets();

	/*
	 * Dialogue Functions
	 */
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FDialogue& GetCurrentDialogue() { return CurrentDialogue; };
	
	UFUNCTION(BlueprintCallable)
	FDialogue FindCurrentDialogueProgress() const;

	UFUNCTION(BlueprintCallable)
	FDialogue FindDialogueRow(UPARAM(ref)FDonDialogueContext& DialogueContext) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasDialogueInPlayerState(UPARAM(ref)FObjective& Objective);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasResponseOptionsInDialogue() const;

	UFUNCTION(BlueprintCallable)
	void AddDialogueToPlayerState(const FDialogue& Dialogue);

	UFUNCTION(BlueprintCallable)
	void SetCurrentDialogue(UPARAM(ref)FDialogue& Dialogue) { CurrentDialogue = Dialogue; };

	
	/*
	 * Quest Functions
	 */

	UFUNCTION(BlueprintCallable)
	bool HasCompletableQuestsInNPC(UPARAM(ref)TArray<FQuest>& OutQuests);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasCompletedQuestInPlayerState(UPARAM(ref)FObjective& Objective) const;
	
	UFUNCTION(BlueprintCallable)
	FQuest FindCompletableQuest(const FString QuestTitle) const;
	
	UFUNCTION(BlueprintCallable)
	void AddQuestToPlayerState(FQuest Quest, EQuestState QuestState = EQuestState::InProgress);

	UFUNCTION(BlueprintCallable)
	void AwardQuestRewardsToPlayerState(FString QuestTitle);

	UFUNCTION(BlueprintCallable)
	FQuest FindQuestInDialogue();

	UFUNCTION(BlueprintCallable)
	bool HasQuestInPlayerState(ENPCName NPCName, FString QuestTitle) const;

	
	/*
	 * Item Functions
	 */
	
	UFUNCTION(BlueprintCallable)
    void AddItemToPlayerState(FName ItemName, int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasItemInPlayerState(UPARAM(ref)FObjective& Objective);


protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UInteractComponent> InteractComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	FDialogue CurrentDialogue;
};
