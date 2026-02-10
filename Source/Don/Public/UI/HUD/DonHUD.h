// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayTagContainer.h"
#include "DonHUD.generated.h"


class UDonWidgetController;
class UInnWidgetController;
class UAttributeMenuWidgetController;
class UQuestListWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UInventoryWidgetController;
class UDonUserWidget;

using FUICommand = TFunction<void()>;

/**
 * 
 */
UCLASS()
class DON_API ADonHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);
	UQuestListWidgetController* GetQuestListWidgetController(const FWidgetControllerParams& WCParams);
	UInnWidgetController* GetInnWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	bool IsValidInViewport(UDonUserWidget* Widget);
	bool CloseWidgetIfOpened(TObjectPtr<UDonUserWidget>& Widget);

	UFUNCTION(BlueprintCallable)
	UDonUserWidget* GetOverlayWidget() const { return OverlayWidget; }

	UFUNCTION(BlueprintImplementableEvent)
	void OpenMenu();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OpenInventory();

	UFUNCTION(BlueprintImplementableEvent)
	void OpenQuests();

	UFUNCTION(BlueprintImplementableEvent)
	void OpenAttributeMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void OpenSkillMenu();
	
	void OpenSetting();
	void OpenDialog();
	void OpenStore();
	void OpenInnRoom();

	void HandleUIOpenRequest(FGameplayTag Tag);
	
	// Inn

	void OpenInnMenu();
	void OpenGroceryStore();

private:
	TMap<FGameplayTag, FUICommand> UICommandMap;

	
	UPROPERTY()
	TObjectPtr<UDonUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDonUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UQuestListWidgetController> QuestListWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestListWidgetController> QuestListWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;


	
	// Inn
	
	UPROPERTY()
	TObjectPtr<UInnWidgetController> InnWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInnWidgetController> InnWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UDonUserWidget> InnMenuWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDonUserWidget> InnMenuWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UDonUserWidget> GroceryStoreWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDonUserWidget> GroceryStoreWidgetClass;
};