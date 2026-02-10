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
	ADonPlayerState* PS = CastChecked<ADonPlayerState>(PlayerState);
	
	PS->OnMoneyChangedDelegate.AddUObject(this, &UDonWidgetController::OnMoneyAdded);
	PS->OnMemoryFragmentChangedDelegate.AddUObject(this, &UDonWidgetController::OnMemoryFragmentAdded);
}

void UDonWidgetController::BroadcastAbilityInfo()
{
	if (!GetDonASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FDonAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(DonAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = DonAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = DonAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	DonAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

UDonAbilitySystemComponent* UDonWidgetController::GetDonASC()
{
	if (DonAbilitySystemComponent == nullptr)
	{
		DonAbilitySystemComponent = Cast<UDonAbilitySystemComponent>(AbilitySystemComponent);
	}
	return DonAbilitySystemComponent;
}

void UDonWidgetController::OnMoneyAdded(int32 NewMoney)
{
	OnMoneyChanged.Broadcast(NewMoney);
}

void UDonWidgetController::OnMemoryFragmentAdded(int32 NewMemoryFragment)
{
	OnMemoryFragmentChanged.Broadcast(NewMemoryFragment);
}
