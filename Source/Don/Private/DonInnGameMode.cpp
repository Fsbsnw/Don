// Fill out your copyright notice in the Description page of Project Settings.


#include "DonInnGameMode.h"

#include "Inn/InnManagerComponent/InnManagerComponent.h"
#include "Player/DonPlayerState.h"

void ADonInnGameMode::AddToRevenue(int32 InMoney)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>())
		{
			PS->AddToMoney(InMoney);			
		}
	}
}

void ADonInnGameMode::AddToReputation(int32 InReputation)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>())
		{
			PS->InnManagerComponent->AddToReputation(InReputation);
		}
	}
}

void ADonInnGameMode::AddToPopularity(int32 InPopularity)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>())
		{
			PS->InnManagerComponent->AddToPopularity(InPopularity);
		}
	}
}

void ADonInnGameMode::AddToInterior(int32 InInterior)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>())
		{
			PS->InnManagerComponent->AddToInterior(InInterior);
		}
	}
}

int32 ADonInnGameMode::GetReputation()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>())
		{
			return PS->InnManagerComponent->GetReputation();
		}
	}
	return 0;
}

int32 ADonInnGameMode::GetPopularity()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>())
		{
			return PS->InnManagerComponent->GetPopularity();
		}
	}
	return 0;
}

int32 ADonInnGameMode::GetInterior()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>())
		{
			return PS->InnManagerComponent->GetInterior();
		}
	}
	return 0;
}
