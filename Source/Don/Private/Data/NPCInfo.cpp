// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/NPCInfo.h"

UTexture2D* UNPCInfo::GetNPCImage(ENPCName NPCName)
{
	for (const FNPCInfoData& Info : NPCInfos)
	{
		if (Info.NPCName == NPCName && Info.NPCImage != nullptr)
		{
			return Info.NPCImage;
		}
	}
	return nullptr;
}
