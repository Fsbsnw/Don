// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/ItemAsset.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "DonEquipmentActor.generated.h"


USTRUCT(BlueprintType)
struct FEquipmentAttributes : public FTableRowBase
{
	GENERATED_BODY()

	bool operator == (const FEquipmentAttributes& Other) const
	{
		return Other.EquipmentName == this->EquipmentName;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName EquipmentName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag EquipmentType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 BaseDamage;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 BaseDefense;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCurveTable> UpgradeCT;
};

UCLASS()
class DON_API ADonEquipmentActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADonEquipmentActor();

	void RequestDestroy();

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);
	
	void InitEquipmentAttributes();
	float GetFinalAttribute() const;

	UPROPERTY(EditDefaultsOnly)
	FName EquipmentName;
	FGameplayTag EquipmentType;
	float EquipmentUpgrade;

	void UpdateUpgradedPoint(int32 InPoint);
	void SetEquipmentInfo(const FItem& NewItem) { EquipmentInfo = NewItem; }
	FItem GetEquipmentInfo() const { return EquipmentInfo; }
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	int32 EquipmentBaseDamage;
	int32 EquipmentBaseDefense;
	float EquipmentUpgradedBonus;
	FItem EquipmentInfo;
};
