// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CuisineAsset.h"
#include "GameFramework/Actor.h"
#include "InnChef.generated.h"

struct FKitchenOrder;

UCLASS()
class DON_API AInnChef : public AActor
{
	GENERATED_BODY()
	
public:	
	AInnChef();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	FORCEINLINE bool IsCooking() const { return bIsCooking; }
	void StartOrder(FKitchenOrder& Order);
	void EndOrder();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* ChefImage;

	UFUNCTION(BlueprintCallable)
	void ChefLevelUp() { ChefLevel += 1; }

	int32 GetChefLevel() const { return ChefLevel; }

private:
	UPROPERTY(EditDefaultsOnly)
	int32 ChefLevel = 1;
	
	bool bIsCooking = false;
	FGuid OrderID;
};