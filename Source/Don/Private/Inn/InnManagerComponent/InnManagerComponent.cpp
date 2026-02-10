// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/InnManagerComponent/InnManagerComponent.h"

UInnManagerComponent::UInnManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInnManagerComponent::AddToReputation(int32 InReputation)
{
	Reputation += InReputation;
	OnReputationChanged.Broadcast(Reputation);
}

void UInnManagerComponent::AddToPopularity(int32 InPopularity)
{
	Popularity += InPopularity;
	OnPopularityChanged.Broadcast(Popularity);
}

void UInnManagerComponent::AddToInterior(int32 InInterior)
{
	Interior += InInterior;
	OnInteriorChanged.Broadcast(Interior);
}
