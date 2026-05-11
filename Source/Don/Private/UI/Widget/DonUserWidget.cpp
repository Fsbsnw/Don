// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/DonUserWidget.h"

void UDonUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}

void UDonUserWidget::AddChildWidget(UDonUserWidget* Child)
{
	Child->ParentWidget = this;
	ChildWidgets.Add(Child);

	if (this->WidgetController)
	{
		Child->SetWidgetController(this->WidgetController);
	}
}

// 본인 포함 자식들 제거
void UDonUserWidget::CloseWidgetAndChildren()
{
	// 부모 위젯이 있는 경우
	if (ParentWidget)
	{
		ParentWidget->ChildWidgets.Remove(this);
		ParentWidget = nullptr;
	}

	// 자식 위젯이 있는 경우
	for (UDonUserWidget* Child : ChildWidgets)
	{
		if (IsValid(Child))
		{
			// 재귀적 파괴
			Child->CloseWidgetAndChildren();
		}
	}
	ChildWidgets.Empty();
	RemoveFromParent();
}

// 본인 제외 자식들 제거
void UDonUserWidget::ClearAllChildren()
{
	// 역순으로 자식들 파괴
	for (int32 i = ChildWidgets.Num() - 1; i >= 0; --i)
	{
		if (IsValid(ChildWidgets[i]))
		{
			// 재귀적 파괴
			ChildWidgets[i]->CloseWidgetAndChildren();
		}
	}
	ChildWidgets.Empty();
}