// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/DonUserWidget.h"

void UDonUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UDonUserWidget::AddChildWidget(UDonUserWidget* Child)
{
	Child->ParentWidget = this;
	ChildWidgets.Add(Child);
}

void UDonUserWidget::CloseWidgetAndChildren()
{
	for (UDonUserWidget* Child : ChildWidgets)
	{
		if (IsValid(Child))
		{
			Child->CloseWidgetAndChildren();
			Child->RemoveFromParent();
			Child->ParentWidget = nullptr;
		}
	}
	ChildWidgets.Empty();
	RemoveFromParent();
}

void UDonUserWidget::CloseSelf()
{
	if (ParentWidget)
	{
		ParentWidget->ChildWidgets.Remove(this);
		ParentWidget = nullptr;
	}

	for (UDonUserWidget* Child : ChildWidgets)
	{
		if (IsValid(Child))
		{
			Child->CloseWidgetAndChildren();
		}
	}
	ChildWidgets.Empty();
	RemoveFromParent();
}
