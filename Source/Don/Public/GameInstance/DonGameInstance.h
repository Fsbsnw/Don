// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Character/Component/InteractComponent.h"
#include "DonGameInstance.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class DON_API UDonGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FDialogueContainer> DialogueDataTable;
};
