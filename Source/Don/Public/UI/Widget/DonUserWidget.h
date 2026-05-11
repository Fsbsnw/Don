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
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintCallable)
	virtual void SetWidgetController(UObject* InWidgetController);

	UFUNCTION(BlueprintCallable)
	void AddChildWidget(UDonUserWidget* Child);
	void CloseWidgetAndChildren();
	void ClearAllChildren();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

	UPROPERTY()
	TObjectPtr<UDonUserWidget> ParentWidget = nullptr;
	UPROPERTY()
	TArray<UDonUserWidget*> ChildWidgets;
};
