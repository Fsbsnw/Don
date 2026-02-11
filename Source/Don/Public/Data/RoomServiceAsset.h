// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoomServiceAsset.generated.h"

USTRUCT(BlueprintType)
struct FRoomServiceOrder
{
	GENERATED_BODY()
	
	bool operator==(const FRoomServiceOrder& Other) const
	{
		return Other.ServiceName == this->ServiceName;
	}

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName ServiceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> ServiceIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 ServiceLevel = 1;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float LimitTime = 10;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 RewardMoney;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> RewardItems;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsRequested = false;
};

/**
 * 
 */
UCLASS()
class DON_API URoomServiceAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RoomServiceInformation")
	TArray<FRoomServiceOrder> RoomServiceInformation;
	
	UFUNCTION(BlueprintCallable)
	FRoomServiceOrder FindRoomServiceByName(FName RoomServiceName);
		
	UFUNCTION(BlueprintCallable)
	FRoomServiceOrder GetRandomRoomService();
};
