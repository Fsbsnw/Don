// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AbilityWidgetController.h"

#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UAbilityWidgetController::BindCallbacksToDependencies()
{
}

void UAbilityWidgetController::BroadcastInitialValues()
{
}

void UAbilityWidgetController::BroadcastAbilityInfo()
{
	UDonAbilitySystemComponent* DASC = Cast<UDonAbilitySystemComponent>(AbilitySystemComponent);
	if (!DASC || !DASC->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this, DASC](const FGameplayAbilitySpec& AbilitySpec)
		{
			FDonAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(DASC->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = DASC->GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = DASC->GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	DASC->ForEachAbility(BroadcastDelegate);
}