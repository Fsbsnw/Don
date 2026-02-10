// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Inn/Character/InnCustomer.h"
#include "CustomerAsset.generated.h"

USTRUCT(BlueprintType)
struct FCustomerData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ECustomerType Type = ECustomerType::First;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AInnCustomer> CustomerClass;
};

/**
 * 
 */
UCLASS(BlueprintType)
class DON_API UCustomerAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FCustomerData> CustomerData;

	TSubclassOf<AInnCustomer> GetCustomerClassByType(ECustomerType Type);
};
