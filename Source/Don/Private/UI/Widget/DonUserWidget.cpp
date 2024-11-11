// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/DonUserWidget.h"

void UDonUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}