// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueQuestCommon.h"
#include "Engine/DataAsset.h"
#include "NPCInfo.generated.h"

USTRUCT(BlueprintType)
struct FNPCInfoData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ENPCName NPCName = ENPCName::Normal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* NPCImage;
};

/**
 * 
 */
UCLASS()
class DON_API UNPCInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FNPCInfoData> NPCInfos;

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetNPCImage(ENPCName NPCName);
};
