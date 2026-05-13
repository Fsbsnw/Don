// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "AbilityWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UAbilityWidgetController : public UDonWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UFUNCTION(BlueprintCallable)
	void BroadcastAbilityInfo();

	UPROPERTY(BlueprintAssignable)
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAbilityInfo> AbilityInfo;
};
