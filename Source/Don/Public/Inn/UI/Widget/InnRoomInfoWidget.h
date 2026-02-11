// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/DonUserWidget.h"
#include "InnRoomInfoWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomServiceChangedUI, float, TimeLimit);

class UInnCustomerGroup;
/**
 * 
 */
UCLASS()
class DON_API UInnRoomInfoWidget : public UDonUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnCustomerBound();
	
public:
	UFUNCTION(BlueprintCallable)
	void BindCustomer(UInnCustomerGroup* Group);

	UPROPERTY(BlueprintAssignable)
	FOnRoomServiceChangedUI OnRoomServiceChangedUI;

	UFUNCTION()
	void OnRoomServiceLimitUpdated(float TimeLimit);

	UPROPERTY(BlueprintReadOnly)
	float MaxTimeLimit;
};
