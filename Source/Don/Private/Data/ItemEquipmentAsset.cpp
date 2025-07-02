// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemEquipmentAsset.h"

#include "Data/ItemStructs.h"

FItemEquipmentInfo UItemEquipmentAsset::FindItemEquipmentByName(FName ItemName)
{
	for (FItemEquipmentInfo Item : EquipmentInformation)
	{
		if (Item.ItemName == ItemName)
		{
			return Item;
		}
	}
	return FItemEquipmentInfo();
}