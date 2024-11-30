// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Dialogue.h"
#include "Data/Quest.h"
#include "Data/ItemAsset.h"
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
	FDialogue SetCurrentDialogueProgress() const;

	UFUNCTION(BlueprintCallable)
	FDialogue FindDialogueRow(const ENPCName& NPCName, const EDialogueFlow& DialogueFlow, const int32 Branch, const int32 Progress) const;

	UFUNCTION(BlueprintCallable)
	void AddItemToPlayerState(const FItem& Item, int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void AddDialogueToPlayerState(const FDialogue& Dialogue);

	UFUNCTION(BlueprintCallable)
	void AddQuestToPlayerState(FQuest Quest, EQuestState QuestState = EQuestState::InProgress);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UInteractComponent> InteractComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;
};
