// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InnWidgetController.h"

#include "DonGameplayTags.h"
#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "Inn/InnManagerComponent/InnManagerComponent.h"
#include "Inn/InnStoreComponent/InnStoreComponent.h"
#include "Inn/Object/InnCustomerGroup.h"
#include "Inn/UI/Widget/InnRoomInfoWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerController.h"
#include "Player/DonPlayerState.h"

void UInnWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();

	// Inn
	InnSystem->OnInnGroupChanged.AddUObject(this, &UInnWidgetController::OnInnGroupUpdated);
	InnSystem->OnInnGroupRoomServiceReceived.AddUObject(this, &UInnWidgetController::OnInnGroupRoomServiceReceived);
	DonPlayerState->InnManagerComponent->OnReputationChanged.AddUObject(this, &UInnWidgetController::OnReputationAdded);
	DonPlayerState->InnManagerComponent->OnPopularityChanged.AddUObject(this, &UInnWidgetController::OnPopularityAdded);
	DonPlayerState->InnManagerComponent->OnInteriorChanged.AddUObject(this, &UInnWidgetController::OnInteriorAdded);

	// Inn Store
	DonPlayerState->GetInnStoreComponent()->OnInnMerchandiseChanged.AddUObject(this, &UInnWidgetController::OnMerchandiseUpdated);

	// Player
	DonPlayerState->GetInventoryComponent()->OnInventoryChanged.AddDynamic(this, &UInnWidgetController::OnPlayerInventoryUpdated);
	DonPlayerState->OnMoneyChangedDelegate.AddUObject(this, &UInnWidgetController::OnMoneyAdded);
}

void UInnWidgetController::BroadcastInitialValues()
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	UInnManagerSubsystem* InnSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInnManagerSubsystem>();

	// Inn
	OnInnGroupChangedUI.Broadcast(InnSystem->GetInnGroupsArray());
	OnPlayerInventoryChanged.Broadcast(DonPlayerState->GetInventoryComponent()->GetInventory());

	// Inn Store
	OnInnMerchandiseChangedUI.Broadcast(DonPlayerState->GetInnStoreComponent()->InnMerchandise);

	// Player
	OnMoneyChanged.Broadcast(DonPlayerState->GetMoney());
	OnReputationChanged.Broadcast(DonPlayerState->InnManagerComponent->GetReputation());
	OnPopularityChanged.Broadcast(DonPlayerState->InnManagerComponent->GetPopularity());
	OnInteriorChanged.Broadcast(DonPlayerState->InnManagerComponent->GetInterior());
}

FInnCustomerGroupSnapshot UInnWidgetController::GetCurrentLodgerInfo(int32 RoomNumber, bool& bOutOccupied, bool& bOutServiceRequested)
{
	UInnManagerSubsystem* InnSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInnManagerSubsystem>();
	FInnCustomerGroupSnapshot GroupSnapshot = InnSystem->GetLodgerInfo(RoomNumber);

	bOutOccupied = InnSystem->RoomInfos[RoomNumber].bIsOccupied;
	bOutServiceRequested = GroupSnapshot.RoomService.bIsRequested;
	
	return GroupSnapshot;
}

UInnCustomerGroup* UInnWidgetController::GetGroupInfo(int32 RoomNumber) const
{
	UInnManagerSubsystem* InnSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInnManagerSubsystem>();
	return InnSystem->GetGroupInfo(InnSystem->GetRoomInfo(RoomNumber).GroupID);
}

FRoomInfo UInnWidgetController::GetRoomInfo(int32 RoomNumber)
{
	UInnManagerSubsystem* InnSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInnManagerSubsystem>();
	return InnSystem->GetRoomInfo(RoomNumber);
}

void UInnWidgetController::OnInnGroupUpdated(const TArray<UInnCustomerGroup*> Groups)
{
	OnInnGroupChangedUI.Broadcast(Groups);
}

void UInnWidgetController::OnInnGroupRoomServiceReceived(int32 GroupID)
{
	OnInnGroupRoomServiceReceivedUI.Broadcast(GroupID);
}

void UInnWidgetController::OnMerchandiseUpdated(const TArray<FItem>& InnMerchandise)
{
	OnInnMerchandiseChangedUI.Broadcast(InnMerchandise);
}

void UInnWidgetController::OnReputationAdded(int32 NewReputation)
{
	OnReputationChanged.Broadcast(NewReputation);
}

void UInnWidgetController::OnPopularityAdded(int32 NewPopularity)
{
	OnPopularityChanged.Broadcast(NewPopularity);
}

void UInnWidgetController::OnInteriorAdded(int32 NewInterior)
{
	OnInteriorChanged.Broadcast(NewInterior);
}

void UInnWidgetController::AddToGroupSatisfaction(int32 RoomNumber, int32 Satisfaction)
{
	UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	int32 GroupID = InnSystem->RoomInfos[RoomNumber].GroupID;
	UInnCustomerGroup* Group = InnSystem->GetGroupInfo(GroupID);

	Group->AddToSatisfaction(Satisfaction);
}

bool UInnWidgetController::IsSuitableForGroup(int32 GroupID, int32 RoomNumber)
{
	UInnManagerSubsystem* InnSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInnManagerSubsystem>();
	if (InnSystem->RoomInfos[RoomNumber].bIsOccupied) return false;	
	
	return InnSystem->GetGroupSize(GroupID) <= InnSystem->RoomInfos[RoomNumber].Size;
}

bool UInnWidgetController::AssignCustomerToRoom(int32 GroupID, int32 RoomNumber, UInnRoomInfoWidget* TargetWidget)
{
	UInnManagerSubsystem* InnSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInnManagerSubsystem>();
	const bool bAssigned = InnSystem->AssignCustomerToRoom(GroupID, RoomNumber);

	if (bAssigned && TargetWidget)
	{
		UInnCustomerGroup* Group = InnSystem->GetGroupInfo(GroupID);
		if (Group)
		{
			TargetWidget->BindCustomer(Group);
			return true;
		}
	}
	return false;
}

void UInnWidgetController::OpenGroceryStore()
{
	if (ADonPlayerController* DPC = Cast<ADonPlayerController>(PlayerController))
	{
		DPC->OnUIOpenRequested.ExecuteIfBound(FDonGameplayTags::Get().UI_Inn_GroceryStore_Open);
		DPC->OnUIOpenRequested.ExecuteIfBound(FDonGameplayTags::Get().UI_Inventory_Open);
	}
}

bool UInnWidgetController::SellMerchandise(int32 ItemIndex)
{
	if (ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState))
	{
		return DonPlayerState->GetInnStoreComponent()->SellMerchandise(ItemIndex);
	}
	return false;
}

void UInnWidgetController::OnPlayerInventoryUpdated(const TArray<FItem>& Inventory)
{
	OnPlayerInventoryChanged.Broadcast(Inventory);
}
