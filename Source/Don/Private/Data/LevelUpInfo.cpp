// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	while (true)
	{
		// LevelUpInformation's Index == Level
		// => LevelUpInformation[10] == Level 10
		// => Size of LevelUpInformation == 11, Level == 10
		// If former == 10 -> Cannot find info
		if (LevelUpInformation.Num() <= Level) break;

		// Example: If InXP == 250, Level 1 requires 300 XP, the correct level is found 
		if (XP < LevelUpInformation[Level].LevelUpRequirement)
		{
			break;
		}

		// Example: If InXP == 350, Level 1 requires 300 XP, continue searching to the next level
		Level++;
	}
	
	return FMath::Clamp(Level, 1, Level - 1);
}
