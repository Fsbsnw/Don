// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayTagContainer.h"
#include "DonHUD.generated.h"


struct FWidgetControllerParams;
class UAttributeSet;
struct FUIConfigData;
class UAbilitySystemComponent;
class UDonUserWidget;
class UDonWidgetController;
/**
 * 
 */
UCLASS()
class DON_API ADonHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UDonWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams, TSubclassOf<UDonWidgetController> WidgetControllerClass);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, const FUIConfigData& UIConfigData);

private:
	UPROPERTY()
	TObjectPtr<UDonUserWidget> OverlayWidget;
	UPROPERTY()
	TObjectPtr<UDonWidgetController> OverlayWidgetController;
};