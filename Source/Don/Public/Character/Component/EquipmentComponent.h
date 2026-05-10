// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemStructs.h"
#include "EquipmentComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentChanged);

class ADonEquipmentActor;
struct FGameplayTag;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TMap<FGameplayTag, TObjectPtr<ADonEquipmentActor>> EquipmentParts;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TMap<FGameplayTag, FName> EquipmentSockets;
	
	void SpawnAndAttachEquipment(const FItem& Item);
	void DetachAndDestroyEquipment(const FItem& Item);
	
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentChanged OnEquipmentChanged;
	
private:
	UPROPERTY()
	ACharacter* OwnerCharacter;
};
