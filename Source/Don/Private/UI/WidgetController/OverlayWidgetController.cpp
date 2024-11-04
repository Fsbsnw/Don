// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UDonAttributeSet* DonAttributeSet = CastChecked<UDonAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(DonAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(DonAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
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

	Cast<UDonAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
			}
		}
	);
}

void UOverlayWidgetController::SetCurrentHealth(float NewHealth)
{
	UDonAttributeSet* DonAttributeSet = CastChecked<UDonAttributeSet>(AttributeSet);

	DonAttributeSet->SetHealth(NewHealth);
	UE_LOG(LogTemp, Warning, TEXT("Set Health Attribute : %f"), NewHealth);
}
