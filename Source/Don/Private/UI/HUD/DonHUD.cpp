// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/DonHUD.h"

#include "DonGameplayTags.h"
#include "Player/DonPlayerController.h"
#include "UI/Widget/DonUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/InnWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/InventoryWidgetController.h"
#include "UI/WidgetController/QuestListWidgetController.h"

void ADonHUD::BeginPlay()
{
	Super::BeginPlay();

	const FDonGameplayTags& DonTags = FDonGameplayTags::Get();
	
	UICommandMap.Add(DonTags.UI_Inventory_Open, [this](){ OpenInventory(); });
	UICommandMap.Add(DonTags.UI_Attribute_Open, [this](){ OpenAttributeMenu(); });
	UICommandMap.Add(DonTags.UI_Skill_Open, [this](){ OpenSkillMenu(); });
	UICommandMap.Add(DonTags.UI_Quest_Open, [this](){ OpenQuests(); });
	UICommandMap.Add(DonTags.UI_Setting_Open, [this](){ OpenSetting(); });
	UICommandMap.Add(DonTags.UI_Interact_Dialog_Open, [this](){ OpenDialog(); });
	UICommandMap.Add(DonTags.UI_Interact_Store_Open, [this](){ OpenStore(); });
	UICommandMap.Add(DonTags.UI_Inn_Menu_Open, [this](){ OpenInnMenu(); });
	UICommandMap.Add(DonTags.UI_Inn_Room_Open, [this](){ OpenInnRoom(); });
	UICommandMap.Add(DonTags.UI_Inn_GroceryStore_Open, [this](){ OpenGroceryStore(); });
}

UOverlayWidgetController* ADonHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* ADonHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

UInventoryWidgetController* ADonHUD::GetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		InventoryWidgetController->BindCallbacksToDependencies();
	}
	return InventoryWidgetController;
}

UQuestListWidgetController* ADonHUD::GetQuestListWidgetController(const FWidgetControllerParams& WCParams)
{
	if (QuestListWidgetController == nullptr)
	{
		QuestListWidgetController = NewObject<UQuestListWidgetController>(this, QuestListWidgetControllerClass);
		QuestListWidgetController->SetWidgetControllerParams(WCParams);
		QuestListWidgetController->BindCallbacksToDependencies();
	}
	return QuestListWidgetController;
}

UInnWidgetController* ADonHUD::GetInnWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InnWidgetController == nullptr)
	{
		InnWidgetController = NewObject<UInnWidgetController>(this, InnWidgetControllerClass);
		InnWidgetController->SetWidgetControllerParams(WCParams);
		InnWidgetController->BindCallbacksToDependencies();
	}
	return InnWidgetController;
}

void ADonHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized"));

	if (ADonPlayerController* DPC = Cast<ADonPlayerController>(PC))
	{
		DPC->OnUIOpenRequested.BindUObject(this, &ADonHUD::HandleUIOpenRequest);
	}
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UDonUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

bool ADonHUD::CloseWidgetIfOpened(TObjectPtr<UDonUserWidget>& Widget)
{
	if (!IsValidInViewport(Widget))
	{
		return false;
	}

	Widget->CloseSelf();
	Widget = nullptr;

	if (APlayerController* PC = GetOwningPlayerController())
	{
		PC->SetInputMode(FInputModeGameAndUI());
	}

	return true;
}

bool ADonHUD::IsValidInViewport(UDonUserWidget* Widget)
{
	return IsValid(Widget) && Widget->IsInViewport();
}

void ADonHUD::HandleUIOpenRequest(FGameplayTag Tag)
{
	if (UICommandMap.Contains(Tag))
	{
		UICommandMap[Tag]();
	}
}

void ADonHUD::OpenInnMenu()
{
	if (CloseWidgetIfOpened(InnMenuWidget) || InnMenuWidgetClass == nullptr) return;
	
	InnMenuWidget = CreateWidget<UDonUserWidget>(GetWorld(), InnMenuWidgetClass);
	InnWidgetController = Cast<UInnWidgetController>(OverlayWidgetController);
	InnMenuWidget->SetWidgetController(InnWidgetController);

	InnMenuWidget->AddToViewport();
}

void ADonHUD::OpenGroceryStore()
{
	if (CloseWidgetIfOpened(GroceryStoreWidget) || GroceryStoreWidgetClass == nullptr) return;
	
	GroceryStoreWidget = CreateWidget<UDonUserWidget>(GetWorld(), GroceryStoreWidgetClass);
	if (IsValid(InnMenuWidget)) InnMenuWidget->AddChildWidget(GroceryStoreWidget);
	GroceryStoreWidget->SetWidgetController(InnWidgetController);

	GroceryStoreWidget->AddToViewport();
}

void ADonHUD::OpenSetting()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Setting"));
}

void ADonHUD::OpenDialog()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Dialog"));
}

void ADonHUD::OpenStore()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Store"));
}

void ADonHUD::OpenInnRoom()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Inn Room"));
}
