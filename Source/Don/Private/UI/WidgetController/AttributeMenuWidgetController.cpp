// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "DonGameplayTags.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/DonPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UDonAttributeSet* AS = Cast<UDonAttributeSet>(AttributeSet);
	check(AS);

	for (TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	DonPlayerState->OnAttributePointsChangedDelegate.AddLambda(
	[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UDonAttributeSet* AS = CastChecked<UDonAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	AttributePointsChangedDelegate.Broadcast(DonPlayerState->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UDonAbilitySystemComponent* DonASC = CastChecked<UDonAbilitySystemComponent>(AbilitySystemComponent);
	DonASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::AddAttributePointsTest()
{
	UDonAbilitySystemComponent* DonASC = CastChecked<UDonAbilitySystemComponent>(AbilitySystemComponent);
	DonASC->AddAttributePointsTest();
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)
{
	FDonAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoSignature.Broadcast(Info);
}
