// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/ItemStructs.h"
#include "StoreWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMerchandiseUpdate, FItem, Item);

struct FItem;
class UInteractComponent;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DON_API UStoreWidgetController : public UObject
{
	GENERATED_BODY()
public:	
	void SetWidgetControllerParams(UInteractComponent* InInteractComponent, APlayerState* TargetPlayerState);
	void BindCallbacksToDependencies();

	UFUNCTION(BlueprintCallable)
	void ResetInteractWidgets();

	UPROPERTY(BlueprintAssignable, Category = "Merchandise")
	FOnMerchandiseUpdate OnMerchandiseUpdate;

	// Player Functions

	UFUNCTION(BlueprintCallable)
	bool PlayerCanAffordItem(int32 Cost);
	
	UFUNCTION(BlueprintCallable)
    void PlayerBuyItem(FItem Item, int32 Amount = 1);

	UFUNCTION(BlueprintCallable)
	void PlayerSellItem(int32 SlotIndex);


	// NPC Functions
	
	AActor* GetNPCCharacter();

	UFUNCTION(BlueprintCallable)
	TArray<FItem> GetNPCMerchandise();

	UFUNCTION(BlueprintCallable)
	void NPCSellMerchandise(int32 SlotIndex);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UInteractComponent> InteractComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;
};
