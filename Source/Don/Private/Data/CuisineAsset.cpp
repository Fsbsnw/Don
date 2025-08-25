// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CuisineAsset.h"

FKitchenOrder UCuisineAsset::FindCuisineByName(FName CuisineName)
{
	for (FKitchenOrder KitchenOrder : CuisineInformation)
	{
		if (KitchenOrder.CuisineName == CuisineName)
		{
			return KitchenOrder;
		}
	}
	return FKitchenOrder();
}