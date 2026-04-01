// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OverlayWidgetController.h"
#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "Data/ItemStructs.h"
#include "InnWidgetController.generated.h"

struct FInnUpgradeResources;
class UInnRoomInfoWidget;
struct FItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInnGroupChangedUI, const TArray<UInnCustomerGroup*>&, CurrentGroups);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInnGroupRoomServiceReceivedUI, int32, GroupID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInventoryChanged, const TArray<FItem>&, Inventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInnMerchandiseChangedUI, const TArray<FItem>&, InnMerchandise);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeResourcesChangedUI, const FInnUpgradeResources&, UpgradeResources);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DON_API UInnWidgetController : public UOverlayWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
	
	UPROPERTY(BlueprintAssignable, Category = "Inn Manager")
	FOnInnMerchandiseChangedUI OnInnMerchandiseChangedUI;
	UPROPERTY(BlueprintAssignable, Category = "Inn Manager")
	FOnInnGroupChangedUI OnInnGroupChangedUI;
	UPROPERTY(BlueprintAssignable, Category = "Inn Manager")
	FOnInnGroupRoomServiceReceivedUI OnInnGroupRoomServiceReceivedUI;
	
	UPROPERTY(BlueprintAssignable, Category = "Inn Manager")
	FOnResourceChanged OnReputationChanged;
	UPROPERTY(BlueprintAssignable, Category = "Inn Manager")
	FOnResourceChanged OnPopularityChanged;
	UPROPERTY(BlueprintAssignable, Category = "Inn Manager")
	FOnResourceChanged OnInteriorChanged;
	UPROPERTY(BlueprintAssignable, Category = "Inn Manager")
	FOnResourceChanged OnSuspicionChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inn Upgrade")
	FOnUpgradeResourcesChangedUI OnTableUpgradeChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Inn Upgrade")
	FOnUpgradeResourcesChangedUI OnChefUpgradeChangedDelegate;


	UFUNCTION(BlueprintCallable)
	FInnCustomerGroupSnapshot GetCurrentLodgerInfo(int32 RoomNumber, UPARAM(ref)bool& bOutOccupied, UPARAM(ref)bool& bOutServiceRequested);

	UFUNCTION(BlueprintCallable)
	UInnCustomerGroup* GetGroupInfo(int32 RoomNumber) const;
	
	UFUNCTION(BlueprintCallable)
	FRoomInfo GetRoomInfo(int32 RoomNumber);


	UFUNCTION()
	void OnInnGroupUpdated(const TArray<UInnCustomerGroup*> Groups);

	UFUNCTION()
	void OnInnGroupRoomServiceReceived(int32 GroupID);
	
	UFUNCTION()
	void OnReputationAdded(int32 NewReputation);

	UFUNCTION()
	void OnPopularityAdded(int32 NewPopularity);
	
	UFUNCTION()
	void OnInteriorAdded(int32 NewInterior);

	UFUNCTION()
	void OnSuspicionAdded(int32 NewSuspicion);

	UFUNCTION()
	void OnTableUpgradeChanged(const FInnUpgradeResources& UpgradeResources);

	UFUNCTION()
	void OnChefUpgradeChanged(const FInnUpgradeResources& UpgradeResources);

	UFUNCTION(BlueprintCallable)
	void AddToGroupSatisfaction(int32 RoomNumber, int32 Satisfaction);

	UFUNCTION(BlueprintCallable)
	bool IsSuitableForGroup(int32 GroupID, int32 RoomNumber);

	UFUNCTION(BlueprintCallable)
	bool AssignCustomerToRoom(int32 GroupID, int32 RoomNumber, UInnRoomInfoWidget* TargetWidget);

	
	// Store

	UFUNCTION(BlueprintCallable)
	void OpenGroceryStore();

	UFUNCTION(BlueprintCallable)
	bool SellMerchandise(int32 ItemIndex);

	UFUNCTION(BlueprintCallable)
	bool UpgradeInn(bool bTargetIsTable);

	UFUNCTION()
	void OnMerchandiseUpdated(const TArray<FItem>& InnMerchandise);
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerInventoryChanged OnPlayerInventoryChanged;
	
	UFUNCTION()
	void OnPlayerInventoryUpdated(const TArray<FItem>& Inventory);
};