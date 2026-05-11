// Fill out your copyright notice in the Description page of Project Settings.


#include "DonInnGameMode.h"

#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"
#include "GameInstance/SubSystem/TimeSubsystem.h"
#include "Inn/Component/InnManagerComponent.h"
#include "Inn/Component/InnStoreComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"

void ADonInnGameMode::BeginPlay()
{
	Super::BeginPlay();

	UTimeSubsystem* TimeSystem = GetGameInstance()->GetSubsystem<UTimeSubsystem>();
	if (TimeSystem)
	{
		TimeSystem->OnMidnight.AddUObject(this, &ADonInnGameMode::HandleMidnight);
		TimeSystem->ResumeTime();
	}

	ADonPlayerState* DPS = Cast<ADonPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	if (DPS && DPS->GetInnStoreComponent())
	{
		DPS->InnStoreComponent->OnDevilPotionPurchased.AddLambda(
			[this]()
			{
				GameOver(2);
			}
		);
	}
}

void ADonInnGameMode::HandleMidnight()
{
	int32 RoomServiceRevenue = 0;
	CloseInnSystem(RoomServiceRevenue);
	
	TArray<FCompletedFoodOrder> CompletedFoodOrders;
	CloseInnKitchen(CompletedFoodOrders);

	EnterMidnightForUI(CompletedFoodOrders, RoomServiceRevenue);
}

void ADonInnGameMode::CloseInnKitchen(TArray<FCompletedFoodOrder>& OutCompletedOrder)
{
	UKitchenOrderSubsystem* KitchenSystem = GetGameInstance()->GetSubsystem<UKitchenOrderSubsystem>();
	if (KitchenSystem)
	{
		OutCompletedOrder = KitchenSystem->CompletedFoodOrders;
		KitchenSystem->CloseKitchen();
	}
}

void ADonInnGameMode::CloseInnSystem(int32& OutRoomServiceRevenue)
{
	UInnManagerSubsystem* InnSystem = GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	if (InnSystem)
	{
		OutRoomServiceRevenue = InnSystem->RoomServiceRevenue;
		InnSystem->CloseInnAtMidnight();
	}
}

void ADonInnGameMode::EnterDungeon()
{
	if (!DungeonLevel.IsNull())
	{
		UTimeSubsystem* TimeSystem = GetGameInstance()->GetSubsystem<UTimeSubsystem>();
		if (TimeSystem)
		{
			TimeSystem->ResumeTime();
		}
		FName LevelName = FName(*DungeonLevel.GetAssetName());
		UGameplayStatics::OpenLevel(this, LevelName);
	}
}

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

void ADonInnGameMode::AddToInnLevel(int32 InInnLevel)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>())
		{
			PS->InnManagerComponent->AddToInnLevel(InInnLevel);
		}
	}
}

void ADonInnGameMode::AddToSuspicion(int32 InSuspicion)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>())
		{
			PS->InnManagerComponent->AddToSuspicion(InSuspicion);
		}
	}
}

void ADonInnGameMode::GameOver(int32 Type)
{
	if (Type == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over : Player has died in the Dungeon."));
	}
	else if (Type == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over : Suspicion reached 100. You have been deported from the Inn!"));
	}
	else
	{
		UTimeSubsystem* TimeSystem = GetGameInstance()->GetSubsystem<UTimeSubsystem>();
		Day = TimeSystem->GetCurrentDay();
		Hour = TimeSystem->GetCurrentHour();
		Minute = TimeSystem->GetCurrentTenMinuteUnit();
		
		UE_LOG(LogTemp, Warning, TEXT("Game Clear! : You’ve turned into a devil."));
	}

	if (APlayerController	* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetShowMouseCursor(true);
		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
	}
	
	GameOverForUI(Type);
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
