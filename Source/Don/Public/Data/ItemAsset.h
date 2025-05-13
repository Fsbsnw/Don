// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ItemAsset.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equipable   UMETA(DisplayName = "Equipable Item"),  
	Consumable  UMETA(DisplayName = "Consumable Item"), 
	Miscellaneous UMETA(DisplayName = "Miscellaneous"), 
	Quest       UMETA(DisplayName = "Quest Item")       
};

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	bool operator==(const FItem& Other) const
	{
		return Other.ItemName == this->ItemName && Other.ItemType == this->ItemType;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	
	UPROPERTY()
	FGuid ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Durability = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Upgrade = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventorySlotIndex = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemType ItemType = EItemType::Miscellaneous;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemPrice = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> ItemActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag ItemTag;
};


/**
 * 
 */
UCLASS()
class DON_API UItemAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInformation")
	TArray<FItem> ItemInformation;
	
	UFUNCTION(BlueprintCallable)
	FItem FindItemByName(FName ItemName);
};
