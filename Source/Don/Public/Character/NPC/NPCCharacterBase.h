// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Dialogue.h"
#include "Character/Interface/InteractInterface.h"
#include "Data/Quest.h"
#include "GameFramework/Character.h"
#include "NPCCharacterBase.generated.h"

class UWidgetComponent;

UCLASS()
class DON_API ANPCCharacterBase : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	ANPCCharacterBase();

	virtual UInteractComponent* GetInteractComponent() override;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact(APlayerState* TargetPlayerState) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	ENPCName NPCName = ENPCName::Normal;

	void NotifyQuestCompletionOnScreen(FQuest Quest);
	void RemoveCompletedQuest(FQuest Quest);
	
	TArray<FQuest> CompletableQuests;
	
	UPROPERTY(EditAnywhere)
	float NormalPriceRate = 1.f;
	
	UPROPERTY(EditAnywhere)
	float EquipmentPriceRate = 1.f;

	UPROPERTY(EditAnywhere)
	float ConsumablePriceRate = 1.f;

	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UInteractComponent> InteractComponent;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UWidgetComponent> QuestNotification;
};
