// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/UI/Widget/InnRoomInfoWidget.h"

#include "Inn/Object/InnCustomerGroup.h"

void UInnRoomInfoWidget::BindCustomer(UInnCustomerGroup* Group)
{
	Group->OnRoomServiceChanged.AddUObject(this, &UInnRoomInfoWidget::OnRoomServiceLimitUpdated);
	OnCustomerBound();
	MaxTimeLimit = Group->MaxRoomServiceTimeLimit;
	OnRoomServiceChangedUI.Broadcast(Group->GroupRoomService.LimitTime);
}

void UInnRoomInfoWidget::OnRoomServiceLimitUpdated(float TimeLimit)
{
	OnRoomServiceChangedUI.Broadcast(TimeLimit);
}