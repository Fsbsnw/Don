// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CuisineAsset.generated.h"

class AInnChef;

USTRUCT(BlueprintType)
struct FKitchenOrder
{
	GENERATED_BODY();

	FKitchenOrder()
	{
		OrderID = FGuid::NewGuid();
	}

	bool operator==(const FKitchenOrder& Other) const
	{
		return Other.CuisineName == this->CuisineName && Other.OrderID == this->OrderID;
	}
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName CuisineName;

	UPROPERTY(BlueprintReadOnly)
	FGuid OrderID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> CuisineIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 CookingTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 Price;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsCooking = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 RemainingTime;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AInnChef> AssignedChef;
};

/**
 * 
 */
UCLASS()
class DON_API UCuisineAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CuisineInformation")
	TArray<FKitchenOrder> CuisineInformation;
	
	UFUNCTION(BlueprintCallable)
	FKitchenOrder FindCuisineByName(FName CuisineName);
};
