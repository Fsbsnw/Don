// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/DonInnLibrary.h"

#include "Data/CuisineAsset.h"
#include "Data/RoomServiceAsset.h"
#include "GameInstance/DonGameInstance.h"
#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"
#include "GameInstance/SubSystem/RoomServiceOrderSubsystem.h"
#include "Inn/Data/CustomerAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"
#include "UI/HUD/DonHUD.h"
#include "UI/WidgetController/DonWidgetController.h"

UInnWidgetController* UDonInnLibrary::GetInnWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(PC->GetHUD()))
		{
			ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DonHUD->GetInnWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

TSubclassOf<AInnCustomer> UDonInnLibrary::GetCustomerClass(const UObject* WorldContextObject, ECustomerType Type)
{
	const UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance == nullptr || DonGameInstance->CustomerDataAsset == nullptr) return nullptr;

	return DonGameInstance->CustomerDataAsset->GetCustomerClassByType(Type);
}

FKitchenOrder UDonInnLibrary::FindCuisineByName(const UObject* WorldContextObject, FName CuisineName)
{
	const UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance == nullptr || DonGameInstance->CuisineDataAsset == nullptr) return FKitchenOrder();

	return DonGameInstance->CuisineDataAsset->FindCuisineByName(CuisineName);
}

FRoomServiceOrder UDonInnLibrary::FindRoomServiceByName(const UObject* WorldContextObject, FName RoomServiceName)
{
	const UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance == nullptr || DonGameInstance->RoomServiceDataAsset == nullptr) return FRoomServiceOrder();

	return DonGameInstance->RoomServiceDataAsset->FindRoomServiceByName(RoomServiceName);
}

FRoomServiceOrder UDonInnLibrary::GetRandomRoomService(const UObject* WorldContextObject)
{
	const UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance == nullptr || DonGameInstance->RoomServiceDataAsset == nullptr) return FRoomServiceOrder();

	return DonGameInstance->RoomServiceDataAsset->GetRandomRoomService();
}

void UDonInnLibrary::AddKitchenOrder(const UObject* WorldContextObject, FKitchenOrder& Order)
{
	UKitchenOrderSubsystem* KitchenOrderSubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UKitchenOrderSubsystem>();
	if (KitchenOrderSubsystem == nullptr) return;

	KitchenOrderSubsystem->EnqueueKitchenOrder(Order);
}

FGuid UDonInnLibrary::FindNextQueuedKitchenOrder(const UObject* WorldContextObject)
{
	UKitchenOrderSubsystem* KitchenOrderSubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UKitchenOrderSubsystem>();
	if (KitchenOrderSubsystem == nullptr) return FGuid();

	return KitchenOrderSubsystem->FindNextQueuedOrderID();
}

void UDonInnLibrary::AddRoomServiceOrder(const UObject* WorldContextObject, FRoomServiceOrder Order)
{
	URoomServiceOrderSubsystem* RoomServiceOrderSubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<URoomServiceOrderSubsystem>();
	if (RoomServiceOrderSubsystem == nullptr) return;

	RoomServiceOrderSubsystem->EnqueueRoomServiceOrder(Order);
}

FRoomServiceOrder UDonInnLibrary::RemoveRoomServiceOrder(const UObject* WorldContextObject)
{
	URoomServiceOrderSubsystem* RoomServiceOrderSubsystem = UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<URoomServiceOrderSubsystem>();
	if (RoomServiceOrderSubsystem == nullptr) return FRoomServiceOrder();

	return RoomServiceOrderSubsystem->DequeueRoomServiceOrder();
}