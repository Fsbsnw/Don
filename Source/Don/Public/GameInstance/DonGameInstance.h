// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DonEquipmentActor.h"
#include "Data/CuisineAsset.h"
#include "Engine/GameInstance.h"
#include "Data/Dialogue.h"
#include "Data/Quest.h"
#include "Player/Interface/SaveableInterface.h"
#include "DonGameInstance.generated.h"

enum class ECustomerType : uint8;
struct FCustomerData;
class UItemConsumableAsset;
class UItemEquipmentAsset;
class UItemAsset;
class UCustomerAsset;
class URoomServiceAsset;
class UCuisineAsset;
class UAbilityInfo;
class UDataTable;
/**
 * 
 */
UCLASS()
class DON_API UDonGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	bool InitDialogueDataTable();
	bool InitQuestDataTable();
	bool InitEquipmentDataTable();
	virtual void Init() override;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<ENPCName, FDialogueContainer> DialogueDataTable;

	UPROPERTY(BlueprintReadOnly)
	TMap<ENPCName, FQuestContainer> QuestDataTable;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FEquipmentAttributes> EquipmentDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* CharacterClassDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAbilityInfo* AbilityInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* MerchandiseDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCuisineAsset* CuisineDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URoomServiceAsset* RoomServiceDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCustomerAsset* CustomerDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Information")
	TObjectPtr<UItemAsset> ItemInfoAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Information")
	TObjectPtr<UItemEquipmentAsset> ItemEquipmentAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Information")
	TObjectPtr<UItemConsumableAsset> ItemConsumableAsset;

	UPROPERTY()
	FPlayerSaveData SavedPlayerData;

	UPROPERTY(EditDefaultsOnly)
	float SubSystemTimeMultiplier = 3.f;

	FCustomerData GetCustomerAssetData(ECustomerType Type);
	FKitchenOrder GetRandomCuisine();
};
