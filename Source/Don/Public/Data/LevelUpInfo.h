// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FDonLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;
	
	UPROPERTY(EditDefaultsOnly)
	int32 SkillPointAward = 1;
	
};

/**
 * 
 */
UCLASS()
class DON_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FDonLevelUpInfo> LevelUpInformation;

	int32 FindLevelForXP(int32 XP) const;
	int32 FindAttributeRewardForLevelUp(int32 PrevLevel, int32 CurrLevel) const;
};
