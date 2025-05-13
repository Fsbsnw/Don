// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "AbilityMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UAbilityMenuWidgetController : public UDonWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
