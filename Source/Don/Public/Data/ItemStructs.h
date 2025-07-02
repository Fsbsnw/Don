#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "ItemStructs.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FItemEquipmentInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> ItemActorClass;
};

USTRUCT(BlueprintType)
struct FEquipmentAttribute
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Durability = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Upgrade = 0;
};

USTRUCT(BlueprintType)
struct FItemConsumableInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> ItemEffectClass;
};

USTRUCT(BlueprintType)
struct FLootableItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ItemName;

	UPROPERTY(EditAnywhere)
	int32 Amount;

	UPROPERTY(EditAnywhere)
	float DropRate;
};

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	bool operator==(const FItem& Other) const
	{
		return Other.ItemName == this->ItemName && Other.ItemTag == this->ItemTag;
	}
	bool IsSameInstance(const FItem& Other) const
	{
		return this->ItemID == Other.ItemID;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	
	UPROPERTY()
	FGuid ItemID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag ItemTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemPrice = 0;
	
	UPROPERTY(BlueprintReadWrite)
	int32 InventorySlotIndex = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FEquipmentAttribute EquipmentAttribute;
};