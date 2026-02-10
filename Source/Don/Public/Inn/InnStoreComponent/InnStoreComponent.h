// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemStructs.h"
#include "InnStoreComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInnMerchandiseChanged, const TArray<FItem>&);

USTRUCT(BlueprintType)
struct FMerchandiseData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemAmount;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInnStoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInnStoreComponent();

	UPROPERTY(EditDefaultsOnly)
	TArray<FMerchandiseData> InitialMerchandise;
	TArray<FItem> InnMerchandise;

	void InitializeStoreMerchandise();
	bool SellMerchandise(int32 ItemIndex);

	FOnInnMerchandiseChanged OnInnMerchandiseChanged;
};
