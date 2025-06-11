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
	OnXPPercentChangedDelegate.Broadcast(DonPlayerState->GetXP());
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
	 * Bind AbilitySystemComponent
	 */

	if (GetDonASC())
	{
		if (GetDonASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetDonASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
			GetDonASC()->AbilitiesChangedDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
		
		GetDonASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);

				}
				OnTagChanged.Broadcast(AssetTags);
			}
		);
	}


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

void UOverlayWidgetController::SetCurrentHealth(float NewHealth)
{
	UDonAttributeSet* DonAttributeSet = CastChecked<UDonAttributeSet>(AttributeSet);

	DonAttributeSet->SetHealth(NewHealth);
	UE_LOG(LogTemp, Warning, TEXT("Set Health : %f"), NewHealth);
}

void UOverlayWidgetController::ResetAbilityInputTag(const FGameplayTag& AbilityTag, const FGameplayTag& NewInputTag)
{
	GetDonASC()->SetAbilityInputTag(AbilityTag, NewInputTag);
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
		
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
	else OnXPPercentChangedDelegate.Broadcast(1.f);
}

void UOverlayWidgetController::AddXPToPlayer(int32 AddXP)
{
	Cast<ADonPlayerState>(PlayerState)->AddToXP(AddXP);
}
