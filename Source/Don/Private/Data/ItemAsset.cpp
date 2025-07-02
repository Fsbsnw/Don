// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemAsset.h"

#include "DonGameplayTags.h"
#include "Data/ItemStructs.h"

FItem UItemAsset::FindItemByName(FName ItemName)
{
	for (FItem Item : ItemInformation)
	{
		if (Item.ItemName == ItemName)
		{
			if (Item.ItemTag.MatchesTag(FDonGameplayTags::Get().Item_Equippable)) Item.ItemID = FGuid::NewGuid();
			return Item;
		}
	}
	return FItem();
}
