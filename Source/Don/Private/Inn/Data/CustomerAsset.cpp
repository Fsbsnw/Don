// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Data/CustomerAsset.h"

TSubclassOf<AInnCustomer> UCustomerAsset::GetCustomerClassByType(ECustomerType Type)
{
	for (FCustomerData Data : CustomerData)
	{
		if (Data.Type == Type) return Data.CustomerClass;
	}
	return nullptr;
}