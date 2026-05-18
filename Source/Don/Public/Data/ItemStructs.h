#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "ItemStructs.generated.h"

class UGameplayEffect;

// 장비 아이템
USTRUCT(BlueprintType)
struct FItemEquipmentInfo
{
	GENERATED_BODY()

	/* 아이템 이름 (Primary Key 역할 - 데이터 식별용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	/* 장착 시 생성될 액터 클래스 (월드에 스폰되는 장비) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftClassPtr<AActor> ItemActorClass;
};

// 장비 아이템 속성
USTRUCT(BlueprintType)
struct FEquipmentAttributeInfo
{
	GENERATED_BODY()

	/* 아이템 이름 (Primary Key 역할 - 데이터 식별용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	
	/* 현재 내구도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Durability = 100.f;

	/* 강화 단계 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Upgrade = 0;
};

// 소비 아이템
USTRUCT(BlueprintType)
struct FItemConsumableInfo
{
	GENERATED_BODY()

	/* 아이템 이름 (Primary Key 역할 - 데이터 식별용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	/* 사용 시 적용될 GameplayEffect 클래스 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftClassPtr<UGameplayEffect> ItemEffectClass;
};

// 드랍 가능한 아이템
USTRUCT(BlueprintType)
struct FItemLootableInfo
{
	GENERATED_BODY()

	/* 아이템 이름 (Primary Key 역할 - 데이터 식별용) */
	UPROPERTY(EditAnywhere)
	FName ItemName;
	
	/* 드랍 수량 */
	UPROPERTY(EditAnywhere)
	int32 Amount;

	/* 드랍 확률 (0.0 ~ 1.0) */
	UPROPERTY(EditAnywhere)
	float DropRate;
};

// 기본 아이템 구조
USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	/* 동일 아이템 비교 (이름 + 태그 기준) */
	bool operator==(const FItem& Other) const
	{
		return Other.ItemName == this->ItemName && Other.ItemTag == this->ItemTag;
	}
	/* 완전 동일 인스턴스 비교 (ID 기반) */
	bool IsSameInstance(const FItem& Other) const
	{
		return this->ItemID == Other.ItemID;
	}

	/* 아이템 이름 (Primary Key 역할 - 데이터 식별용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	/* 고유 인스턴스 ID (런타임 식별용) */
	UPROPERTY()
	FGuid ItemID;

	/* UI 아이콘 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon = nullptr;
	
	/* 아이템 타입 태그 (Consumable / Equipable 등 분류) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag ItemTag;

// =========================================================
// Runtime Data
// =========================================================

	/* 현재 수량 (스택용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 1;

	/* 상점 가격 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemPrice = 0;

	/* 인벤토리 슬롯 위치 (UI 동기화용) */
	UPROPERTY(BlueprintReadWrite)
	int32 InventorySlotIndex = -1;
};