// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DonHUD.generated.h"

class UAttributeMenuWidgetController;
class UQuestListWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UInventoryWidgetController;
class UDonUserWidget;
/**
 * 
 */
UCLASS()
class DON_API ADonHUD : public AHUD
{
	GENERATED_BODY()
public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);
	UQuestListWidgetController* GetQuestListWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

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

private:
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
};
