// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Object/InnCustomerGroup.h"

#include "DonInnGameMode.h"
#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "GameInstance/SubSystem/TimeSubsystem.h"
#include "Inn/DonInnLibrary.h"
#include "Kismet/GameplayStatics.h"

void UInnCustomerGroup::OnMemberNotified(ECustomerNotify Notify)
{
	switch (Notify)
	{
	case ECustomerNotify::EnterInn:
		++EnteredMemberCount;
		if (EnteredMemberCount == MemberActors.Num())
		{
			RequestSeats();
		}
		break;
		
	case ECustomerNotify::FinishedEating:
		++FinishedEatingCount;
		if (FinishedEatingCount == MemberActors.Num())
		{
			FinishedEating();
		}
		break;

	case ECustomerNotify::EnterRoom:
		++EnteredRoomMemberCount;
		if (EnteredRoomMemberCount == MemberActors.Num())
		{
			EnterRoom();
		}
		break;
		
	case ECustomerNotify::ExitInn:
		++ExitMemberCount;
		if (ExitMemberCount == MemberActors.Num())
		{
			ExitInn();
		}
		break;
	}
}

void UInnCustomerGroup::RequestSeats()
{
	UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	const bool bFoundEmptySeats = InnSystem->FindAndOccupyEmptySeat(GroupID);

	for (AInnCustomer* Member : MemberActors)
	{
		Member->OnSeatAssigned(bFoundEmptySeats);
	}

	InnState = ECustomerInnState::Kitchen;
}

void UInnCustomerGroup::FinishedEating()
{
	UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	
	MealState = ECustomerMealState::FinishedEating;
	InnSystem->ReturnGroupSeats(GroupID);
	
	for (AInnCustomer* Member : MemberActors)
	{
		Member->OnGroupMealFinished(true);
	}
	
	DecideToLodge();
	OnGroupChanged.Broadcast(GroupID);
}

void UInnCustomerGroup::DecideToLodge()
{
	if (InnState == ECustomerInnState::Entrance) return;

	UTimeSubsystem* TimeSystem = GetWorld()->GetGameInstance()->GetSubsystem<UTimeSubsystem>();
	
	int32 Temp = FMath::RandRange(1, 100);
	bool bHasEmptyRoom = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>()->HasEmptyRoom();
	
	for (AInnCustomer* Member : MemberActors)
	{
		if (bHasEmptyRoom && Temp <= 50 + Satisfaction && TimeSystem->GetCurrentHour() >= 9)
		{
			UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
			const bool bCanCheckIn = InnSystem->CheckInCustomer(GroupID);
			if (bCanCheckIn)
			{
				Member->OnGroupDecidedToStay(true);
				InnState = ECustomerInnState::Room;
			}
			else
			{
				Member->OnGroupDecidedToStay(false);
				InnState = ECustomerInnState::Exit;
			}
		}
		else
		{
			Member->OnGroupDecidedToStay(false);
			InnState = ECustomerInnState::Exit;
		}
	}
}

void UInnCustomerGroup::AddToSatisfaction(int32 InSatisfaction)
{
	Satisfaction += InSatisfaction;
	OnGroupChanged.Broadcast(GroupID);
}

FCustomerSnapshot UInnCustomerGroup::MakeCustomerSnapshot(AInnCustomer* Customer)
{
	ECustomerType Type = Customer->GetType();
	int32 Level = Customer->GetLevel();

	FCustomerSnapshot Snapshot;
	Snapshot.Type = Type;
	Snapshot.Level = Level;

	return Snapshot;
}

void UInnCustomerGroup::EnterRoom()
{
	for (AInnCustomer* Member : MemberActors)
	{
		FCustomerSnapshot Snapshot = MakeCustomerSnapshot(Member);
		MemberSnapshots.Add(Snapshot);

		Member->Destroy();
	}
}

void UInnCustomerGroup::RespawnMembers()
{
	for (FCustomerSnapshot& MemberSnapshot : MemberSnapshots)
	{
		FCustomerData CustomerData = UDonInnLibrary::GetCustomerAssetData(this, MemberSnapshot.Type);
		UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
		FVector RandomOffset(
			FMath::FRandRange(-25.f, 25.f),
			FMath::FRandRange(-25.f, 25.f),
			0.f
		);
		FTransform SpawnTransform(
			FRotator::ZeroRotator,
			InnSystem->RoomEntranceLocation + RandomOffset
		);
		
		AInnCustomer* Customer = GetWorld()->SpawnActor<AInnCustomer>(CustomerData.CustomerClass, SpawnTransform);
		if (Customer)
		{
			Customer->OnGroupMealFinished(true);
			Customer->OnGroupDecidedToStay(false);
		}
		MemberActors.Add(Customer);
	}
}

void UInnCustomerGroup::PayCost()
{
	if (AssignedRoomNumber == -1) return;
	
	UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	FRoomInfo RoomInfo = InnSystem->GetRoomInfo(AssignedRoomNumber);
	int32 Cost = (RoomInfo.BasePrice + RoomInfo.ExtraPrice) * RoomInfo.Grade + 100 * Satisfaction;
	
	ADonInnGameMode* InnGameMode = Cast<ADonInnGameMode>(UGameplayStatics::GetGameMode(this));
	InnGameMode->AddToRevenue(Cost);
}

void UInnCustomerGroup::CheckOut()
{
	RespawnMembers();
	PayCost();
	
	InnState = ECustomerInnState::Exit;
}

void UInnCustomerGroup::SetRoomServiceTimer()
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &UInnCustomerGroup::RequestRoomService);
	int32 Delay = FMath::RandRange(3, 7);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Delay, false);
}

void UInnCustomerGroup::InitRoomService()
{
	GroupRoomService = UDonInnLibrary::GetRandomRoomService(this);
	MaxRoomServiceTimeLimit = GroupRoomService.LimitTime;

	UE_LOG(LogTemp, Warning, TEXT("Init Room Service"));
}

void UInnCustomerGroup::RequestRoomService()
{
	GroupRoomService.bIsRequested = true;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &UInnCustomerGroup::TickRoomService);
	GetWorld()->GetTimerManager().SetTimer(RoomServiceTimeLimitHandle, TimerDelegate, 0.01f, true);
	
	OnGroupRoomServiceRequested.Broadcast(GroupID);
}

void UInnCustomerGroup::TickRoomService()
{
	GroupRoomService.LimitTime -= GetWorld()->GetDeltaSeconds();
	if (GroupRoomService.LimitTime <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(RoomServiceTimeLimitHandle);
		OnRoomServiceChanged.Broadcast(GroupRoomService.LimitTime);
		OnRoomServiceChanged.Clear();
		return;
	}
	OnRoomServiceChanged.Broadcast(GroupRoomService.LimitTime);
}

void UInnCustomerGroup::SetMiniGameSuccess(bool bIsSuccess)
{
	GroupRoomService.bIsMiniGameSuccess = bIsSuccess;
	OnRoomServiceChanged.Clear();
}

void UInnCustomerGroup::ExitInn()
{
	for (AInnCustomer* Member : MemberActors)
	{
		if (!IsValid(Member)) continue;
		
		Member->Destroy();
	}
	MemberActors.Empty();

	MarkAsGarbage();
}
