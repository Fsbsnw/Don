// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"
#include "Player/DonPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UDonAttributeSet* DonAttributeSet = CastChecked<UDonAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(DonAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(DonAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UDonAttributeSet* DonAttributeSet = CastChecked<UDonAttributeSet>(AttributeSet);
	

	/*
	 * Bind Attribute Set
	 */

	
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

	Cast<UDonAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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


	/*
	 * Bind Player State
	 */
	
	Cast<ADonPlayerState>(PlayerState)->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
}

void UOverlayWidgetController::SetCurrentHealth(float NewHealth)
{
	UDonAttributeSet* DonAttributeSet = CastChecked<UDonAttributeSet>(AttributeSet);

	DonAttributeSet->SetHealth(NewHealth);
	UE_LOG(LogTemp, Warning, TEXT("Set Health : %f"), NewHealth);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	int32 XPBarPercent = NewXP;
	
	OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
}

void UOverlayWidgetController::AddXPToPlayer(int32 AddXP)
{
	Cast<ADonPlayerState>(PlayerState)->AddToXP(AddXP);
}
