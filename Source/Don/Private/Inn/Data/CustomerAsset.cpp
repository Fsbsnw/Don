// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Data/CustomerAsset.h"

FCustomerData UCustomerAsset::GetCustomerDataByType(ECustomerType Type)
{
	for (FCustomerData& Data : CustomerData)
	{
		if (Data.Type == Type) return Data;
	}
	return FCustomerData();
}