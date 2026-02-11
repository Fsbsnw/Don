// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SubSystem/InnManagerSubsystem.h"

#include "DonInnGameMode.h"
#include "GameInstance/SubSystem/TimeSubsystem.h"
#include "Inn/Actor/InnSeat.h"
#include "Kismet/GameplayStatics.h"
#include "Inn/Object/InnCustomerGroup.h"


void UInnManagerSubsystem::InitInformation()
{
	ADonInnGameMode* InnGameMode = Cast<ADonInnGameMode>(UGameplayStatics::GetGameMode(this));
	if (InnGameMode)
	{
		RoomInfos = InnGameMode->RoomInfos;
		RoomCapacity = InnGameMode->RoomInfos.Num();
	}

	UTimeSubsystem* TimeSubsystem = GetGameInstance()->GetSubsystem<UTimeSubsystem>();
	if (TimeSubsystem)
	{
		TimeSubsystem->OnMorning.AddUObject(this, &UInnManagerSubsystem::MorningCheckOut);
	}
	
	
	InnGroupFirstNames = {
		TEXT("미숙한"),
		TEXT("평범한"),
		TEXT("숙련된")
	};

	InnGroupSecondNames = {
		TEXT("지방의"),
		TEXT("대도시"),
		TEXT("귀족")
	};

	InnGroupThirdNames = {
		TEXT("사냥꾼"),
		TEXT("탐험가"),
		TEXT("상인")
	};
	
	TArray<AActor*> Exits;
	UGameplayStatics::GetAllActorsWithTag(this, FName("Exit"), Exits);

	for (const AActor* Target : Exits) ExitLocation = Target->GetActorLocation();

	
	TArray<AActor*> InnEntrances;
	UGameplayStatics::GetAllActorsWithTag(this, FName("InnEntrance"), InnEntrances);

	for (const AActor* Target : InnEntrances) InnEntranceLocation = Target->GetActorLocation();

	
	TArray<AActor*> RoomEntrances;
	UGameplayStatics::GetAllActorsWithTag(this, FName("RoomEntrance"), RoomEntrances);

	for (const AActor* Target : RoomEntrances) RoomEntranceLocation = Target->GetActorLocation();

	TArray<AActor*> SeatActors;
	UGameplayStatics::GetAllActorsOfClass(this, AInnSeat::StaticClass(), SeatActors);

	SeatGroups.SetNum(MaxSeatGroupSize);
	
	for (AActor* Actor : SeatActors)
	{
		if (AInnSeat* Seat = Cast<AInnSeat>(Actor))
		{
			SeatGroups[Seat->SeatGroup].Seats.Add(Seat);
			SeatGroups[Seat->SeatGroup].GroupSize++;
		}
	}
}

void UInnManagerSubsystem::SpawnAndRegisterCustomerGroup(TSubclassOf<AInnCustomer> CustomerClass, FVector SpawnLocation, int32 Size)
{
	int32 CurrentGroupID = NextGroupID;
	FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);

	UInnCustomerGroup* NewGroup = NewObject<UInnCustomerGroup>(this);
	NewGroup->GroupID = CurrentGroupID;
	NewGroup->GroupName = NewCustomerName();
	NewGroup->OnGroupChanged.AddUObject(this, &UInnManagerSubsystem::UpdateInnGroup);
	NewGroup->OnGroupRoomServiceRequested.AddUObject(this, &UInnManagerSubsystem::OnRoomServiceReceived);
	
	for (int32 i = 1; i <= Size; ++i)
	{
		FVector RandomOffset(
			FMath::FRandRange(-100.f, 100.f),
			FMath::FRandRange(-100.f, 100.f),
			0.f
		);

		FTransform RandomTransform(
			SpawnTransform.GetRotation(),
			SpawnTransform.GetLocation() + RandomOffset
		);
		
		AInnCustomer* Member = GetWorld()->SpawnActor<AInnCustomer>(CustomerClass, RandomTransform);
		if (Member)
		{
			Member->SetGroupID(CurrentGroupID);
			Member->OnCustomerChanged.AddUObject(NewGroup, &UInnCustomerGroup::OnMemberNotified);
			NewGroup->MemberActors.Add(Member);
		}
	}
	int32 PortraitIndex = FMath::RandRange(0, NewGroup->MemberActors[0]->Portraits.Num() - 1);
	NewGroup->Portrait = NewGroup->MemberActors[0]->Portraits[PortraitIndex];
	InnGroups.Add(CurrentGroupID, NewGroup);
	
	++NextGroupID;
	++TodayCustomerGroupCount;
	
	// UI
	OnInnGroupChanged.Broadcast(GetInnGroupsArray());
}

bool UInnManagerSubsystem::FindAndOccupyEmptySeat(int32 GroupID)
{
	UInnCustomerGroup** GroupPtr = InnGroups.Find(GroupID);
	if (!GroupPtr || !*GroupPtr) return false;

	UInnCustomerGroup* Group = *GroupPtr;
	const int32 GroupSize = Group->MemberActors.Num();
	
	for (int32 i = 0; i < SeatGroups.Num(); ++i)
	{
		FSeatGroup& SeatGroup = SeatGroups[i];
		// 빈 좌석 없으면 다음 좌석
		const int32 SeatSize = SeatGroup.Seats.Num();
		if (SeatGroup.AssignedGroupID != -1 || SeatSize < GroupSize) continue;

		// 빈 좌석 있으면 각 멤버별로 할당
		Group->AssignedTable = i;
		SeatGroup.AssignedGroupID = GroupID;
		TArray<AInnCustomer*>& Members = Group->MemberActors;
		for (AInnSeat* Seat : SeatGroup.Seats)
		{
			for (AInnCustomer* Member : Members)
			{
				if (!IsValid(Member) || Member->Seat != nullptr || Seat->GetIsOccupied() == true) continue;

				Seat->SetIsOccupied(true);
				Member->ReserveSeat(Seat);
			}
		}
		// 모든 멤버 할당 완료시 성공
		return true;
	}
	// 못 찾으면 실패
	return false;
}

void UInnManagerSubsystem::UpdateInnGroup(int32 GroupID)
{
	if (!InnGroups.Contains(GroupID)) return;
	OnInnGroupChanged.Broadcast(GetInnGroupsArray());
}

void UInnManagerSubsystem::OnRoomServiceReceived(int32 GroupID)
{
	if (!InnGroups.Contains(GroupID)) return;
	UE_LOG(LogTemp, Warning, TEXT("Group %d : Requested Room service."), GroupID);
	OnInnGroupRoomServiceReceived.Broadcast(GroupID);
}

void UInnManagerSubsystem::ReturnGroupSeats(int32 GroupID)
{
	if (!InnGroups.Contains(GroupID)) return;
	SeatGroups[InnGroups[GroupID]->AssignedTable].AssignedGroupID = -1;
}

void UInnManagerSubsystem::RemoveGroup(int32 GroupID)
{
	for (FRoomInfo& RoomInfo : RoomInfos)
	{
		if (RoomInfo.GroupID == GroupID)
		{
			RoomInfo.bIsOccupied = false;
			RoomInfo.GroupID = -1;
		}
	}
	InnGroups.Remove(GroupID);
	OnInnGroupChanged.Broadcast(GetInnGroupsArray());
}

TArray<UInnCustomerGroup*> UInnManagerSubsystem::GetInnGroupsArray() const
{
	TArray<UInnCustomerGroup*> GroupsForUI;
	for (auto [Key, Value] : InnGroups)
	{
		GroupsForUI.Add(Value);
	}

	GroupsForUI.StableSort([](const UInnCustomerGroup& A, const UInnCustomerGroup& B)
	{
		return A.InnState > B.InnState;
	});
	
	return GroupsForUI;
}

ECustomerInnState UInnManagerSubsystem::GetGroupInnState(int32 GroupID)
{
	if (!InnGroups.Contains(GroupID)) return ECustomerInnState::Exit;
	return InnGroups[GroupID]->InnState;
}

int32 UInnManagerSubsystem::GetGroupSize(int32 GroupID)
{
	if (!InnGroups.Contains(GroupID)) return 10;
	return InnGroups[GroupID]->MemberActors.Num(); 
}

int32 UInnManagerSubsystem::GetGroupTable(int32 GroupID)
{
	if (!InnGroups.Contains(GroupID)) return -1;
	return InnGroups[GroupID]->AssignedTable;
}

void UInnManagerSubsystem::SetGroupTable(int32 GroupID, int32 TableNumber)
{
	if (!InnGroups.Contains(GroupID)) return;
	InnGroups[GroupID]->AssignedTable = TableNumber;
}

FInnCustomerGroupSnapshot UInnManagerSubsystem::GetLodgerInfo(int32 RoomNumber)
{
	int32 GroupID = RoomInfos[RoomNumber].GroupID;
	if (GroupID == -1 || !InnGroups.Contains(GroupID)) return FInnCustomerGroupSnapshot();

	UInnCustomerGroup* Group = InnGroups[GroupID];
	
	FInnCustomerGroupSnapshot Lodger;
	Lodger.GroupID = GroupID;
	Lodger.GroupPortrait = Group->Portrait;
	Lodger.GroupSize = Group->MemberActors.Num();
	Lodger.SatisfactionScore = Group->Satisfaction;
	Lodger.RoomService = Group->GroupRoomService;
	
	return Lodger;
}

bool UInnManagerSubsystem::CheckInCustomer(int32 GroupID)
{
	if (!InnGroups.Contains(GroupID) || LodgerGroups.Num() >= RoomCapacity) return false;

	LodgerGroups.Add(GroupID);
	return true;
}

void UInnManagerSubsystem::MorningCheckOut()
{
	for (int32 LodgerGroup : LodgerGroups)
	{
		int32 RoomNumber = InnGroups[LodgerGroup]->AssignedRoomNumber;
		if (RoomNumber != -1)
		{
			RoomInfos[InnGroups[LodgerGroup]->AssignedRoomNumber].bIsOccupied = false;
			RoomInfos[InnGroups[LodgerGroup]->AssignedRoomNumber].GroupID = -1;
		}	
		InnGroups[LodgerGroup]->CheckOut();
		InnGroups.Remove(LodgerGroup);
	}
	LodgerGroups.Empty();
	OnInnGroupChanged.Broadcast(GetInnGroupsArray());
}

bool UInnManagerSubsystem::AssignCustomerToRoom(int32 GroupID, int32 RoomIndex)
{
	UInnCustomerGroup* Group = GetGroupInfo(GroupID);
	if (!Group || Group->AssignedRoomNumber != -1) return false;

	Group->InitRoomService();
	Group->AssignedRoomNumber = RoomIndex;
	RoomInfos[RoomIndex].GroupID = GroupID;
	RoomInfos[RoomIndex].bIsOccupied = true;
	Group->SetRoomServiceTimer();
	return true;
}

FString UInnManagerSubsystem::NewCustomerName()
{
	FString NewName;
	
	// First
	int32 FirstRIndex = FMath::RandRange(0, InnGroupFirstNames.Num() - 1);
	NewName += InnGroupFirstNames[FirstRIndex] + FString(" ");
	
	// Second
	int32 SecondRIndex = FMath::RandRange(0, InnGroupSecondNames.Num() - 1);
	NewName += InnGroupSecondNames[SecondRIndex] + FString(" ");
	
	// Third
	int32 ThirdRIndex = FMath::RandRange(0, InnGroupThirdNames.Num() - 1);
	NewName += InnGroupThirdNames[ThirdRIndex];

	return NewName;
}

UInnCustomerGroup* UInnManagerSubsystem::GetGroupInfo(int32 GroupID) const
{
	if (!InnGroups.Contains(GroupID)) return nullptr;
	return InnGroups[GroupID];
}