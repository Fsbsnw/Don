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

FKitchenOrder UCuisineAsset::GetRandomCuisine()
{
	if (CuisineInformation.Num() <= 0) return FKitchenOrder();
	int32 Index = FMath::RandRange(0, CuisineInformation.Num() - 1);
	return CuisineInformation[Index];
}
