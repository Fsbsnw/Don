// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/DonHUD.h"

#include "DonGameplayTags.h"
#include "Data/UIConfigDataAsset.h"
#include "Inventory/DonItemLibrary.h"
#include "Player/DonPlayerController.h"
#include "UI/Widget/DonUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/InnWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/WidgetController/QuestListWidgetController.h"

UDonWidgetController* ADonHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams, TSubclassOf<UDonWidgetController> WidgetControllerClass)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UDonWidgetController>(this, WidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void ADonHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, const FUIConfigData& UIConfigData)
{
	TSubclassOf<UDonUserWidget> OverlayWidgetClass = UIConfigData.OverlayWidgetClass;
	TSubclassOf<UDonWidgetController> OverlayWidgetControllerClass = UIConfigData.OverlayWidgetControllerClass;
	if (OverlayWidgetClass == nullptr || OverlayWidgetControllerClass == nullptr) return;
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UDonUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UDonWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams, OverlayWidgetControllerClass);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}