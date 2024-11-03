// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemAsset.h"

FItem UItemAsset::FindItemByName(FName ItemName)
{
	for (FItem Item : ItemInformation)
	{
		if (Item.ItemName == ItemName)
		{
			return Item;
		}
	}
	return FItem();
}
