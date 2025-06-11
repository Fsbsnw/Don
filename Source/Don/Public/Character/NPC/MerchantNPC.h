// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NPC/NPCCharacterBase.h"
#include "Data/ItemAsset.h"
#include "MerchantNPC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMerchandiseChanged, FItem, Item);

USTRUCT(BlueprintType)
struct FMerchandiseInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Amount = 1;
};

USTRUCT(BlueprintType)
struct FMerchandise : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ENPCName NPCName = ENPCName::Merchant;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FMerchandiseInfo> Merchandise;
};

struct FItem;
/**
 * 
 */
UCLASS()
class DON_API AMerchantNPC : public ANPCCharacterBase
{
	GENERATED_BODY()
public:
	virtual void Interact(APlayerState* TargetPlayerState) override;
	virtual void BeginPlay() override;

	FOnMerchandiseChanged OnMerchandiseChanged;

	UFUNCTION(BlueprintCallable, Category = "Merchandise")
	TArray<FItem>& GetMerchandise() { return Merchandise; }

	void RemoveMerchandise(FItem Item, int32 Amount);
	void LoadMerchandise();

	UPROPERTY(EditAnywhere)
	bool bLoadBonusMerchandise = true;
	float LifeTimer = 10.f;
private:
	UPROPERTY(EditAnywhere)
	TArray<FItem> Merchandise;
	
	UPROPERTY(EditAnywhere)
	TMap<FName, int32> ItemsToAdd;
};
