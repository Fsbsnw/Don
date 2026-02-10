// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DonUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DON_API UDonUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UDonUserWidget* ParentWidget = nullptr;

	UPROPERTY()
	TArray<UDonUserWidget*> ChildWidgets;	
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintCallable)
	virtual void SetWidgetController(UObject* InWidgetController);

	void AddChildWidget(UDonUserWidget* Child);
	void CloseWidgetAndChildren();
	void CloseSelf();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
