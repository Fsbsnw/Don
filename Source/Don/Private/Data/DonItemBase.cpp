// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DonItemBase.h"

#include "Data/ItemStructs.h"


bool UDonItemBase::UseItem(AActor* Target, FItem& Item, bool& bWasConsumed)
{
	UE_LOG(LogTemp, Warning, TEXT("This item cannot be used : %s"), *Item.ItemName.ToString());
	return false;
}