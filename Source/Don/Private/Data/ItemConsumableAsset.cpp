// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemConsumableAsset.h"

#include "Data/ItemStructs.h"


FItemConsumableInfo UItemConsumableAsset::FindItemConsumableByName(FName ItemName)
{
	for (FItemConsumableInfo Item : ConsumableInformation)
	{
		if (Item.ItemName == ItemName)
		{
			return Item;
		}
	}
	return FItemConsumableInfo();
}
