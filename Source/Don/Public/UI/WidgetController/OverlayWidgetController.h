// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FDonAbilityInfo;
class UDonAbilitySystemComponent;
class UAbilityInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerLevelChangedSignature, int32, NewLevel, bool, bLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTagChangedSignature, const FGameplayTagContainer&, TagContainer);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DON_API UOverlayWidgetController : public UDonWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float NewHealth);

	UFUNCTION(BlueprintCallable)
	void ResetAbilityInputTag(const FGameplayTag& AbilityTag, const FGameplayTag& NewInputTag = FGameplayTag());

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnPlayerLevelChangedSignature OnPlayerLevelChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | Abilities")
	FOnTagChangedSignature OnTagChanged;
	
	void OnXPChanged(int32 NewXP);

	UFUNCTION(BlueprintCallable)
	void AddXPToPlayer(int32 AddXP);

protected:
};