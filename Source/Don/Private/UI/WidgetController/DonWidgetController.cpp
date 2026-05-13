// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/DonWidgetController.h"

#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/DonPlayerState.h"

void UDonWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UDonWidgetController::BroadcastInitialValues()
{
}

void UDonWidgetController::BindCallbacksToDependencies()
{
}