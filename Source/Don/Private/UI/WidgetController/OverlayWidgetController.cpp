// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "DonGameplayTags.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Data/LevelUpInfo.h"
#include "Player/DonPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UDonAttributeSet* DonAttributeSet = CastChecked<UDonAttributeSet>(AttributeSet);
	const ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	OnHealthChanged.Broadcast(DonAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(DonAttributeSet->GetMaxHealth());
	
	OnPlayerLevelChangedDelegate.Broadcast(DonPlayerState->GetPlayerLevel(), true);
	OnXPChangedDelegate.Broadcast(DonPlayerState->GetXP());
	OnGameScoreChangedDelegate.Broadcast(DonPlayerState->GetGameScore());
	OnKillCountChangedDelegate.Broadcast(DonPlayerState->GetKillCount());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	/*
	 * Bind Attribute Set
	 */

	const UDonAttributeSet* DonAttributeSet = CastChecked<UDonAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	DonAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	DonAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);


	/*
	 * Bind Player State
	 */

	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	DonPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	DonPlayerState->OnLevelChangedDelegate.AddLambda(
	[this](int32 NewLevel, bool bLevelUp)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
		}
	);
	DonPlayerState->OnGameScoreChangedDelegate.AddLambda(
		[this](int32 NewGameScore)
		{
			OnGameScoreChangedDelegate.Broadcast(NewGameScore);
		}
	);
	DonPlayerState->OnKillCountChangedDelegate.AddLambda(
		[this](int32 NewKillCount)
		{
			OnKillCountChangedDelegate.Broadcast(NewKillCount);
		}
	);
}

void UOverlayWidgetController::ResetAbilityInputTag(const FGameplayTag& AbilityTag, const FGameplayTag& NewInputTag)
{
	if (UDonAbilitySystemComponent* DASC = Cast<UDonAbilitySystemComponent>(AbilitySystemComponent))
	{
		DASC->SetAbilityInputTag(AbilityTag, NewInputTag);
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = CastChecked<ADonPlayerState>(PlayerState)->LevelUpInfo;
	const int32 CurrLevel = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num() - 1;

	if (CurrLevel < MaxLevel)
	{
		const int32 CurrLevelUpRequirement = LevelUpInfo->LevelUpInformation[CurrLevel].LevelUpRequirement;
		const int32 NextLevelUpRequirement = LevelUpInfo->LevelUpInformation[CurrLevel + 1].LevelUpRequirement;

		const int32 XPForThisLevel = NewXP - CurrLevelUpRequirement;
		const int32 DeltaLevelRequirement = NextLevelUpRequirement - CurrLevelUpRequirement;
		
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		
		OnXPChangedDelegate.Broadcast(XPBarPercent);
	}
	else OnXPChangedDelegate.Broadcast(1.f);
}