// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Object/InnCustomerGroup.h"

#include "DonInnGameMode.h"
#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "GameInstance/SubSystem/TimeSubsystem.h"
#include "Inn/DonInnLibrary.h"
#include "Kismet/GameplayStatics.h"


// ============================================================================
// 공통 및 관리 함수 (Common & Management)
// ============================================================================
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

void UInnCustomerGroup::ExitInn()
{
	// 남아있는 멤버 액터 제거
	for (AInnCustomer* Member : MemberActors)
	{
		if (!IsValid(Member)) continue;
		
		Member->Destroy();
	}
	MemberActors.Empty();

	// UObject 가비지 컬렉션 대상 지정
	MarkAsGarbage();
}

// ============================================================================
// 식당 서비스
// ============================================================================
void UInnCustomerGroup::RequestSeats()
{
	UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	if (!InnSystem) return;

	// 빈 좌석 탐색 및 선점
	const bool bFoundEmptySeats = InnSystem->FindAndOccupyEmptySeat(GroupID);

	// 모든 멤버에게 좌석 배정 결과 통보
	for (AInnCustomer* Member : MemberActors)
	{
		Member->OnSeatAssigned(bFoundEmptySeats);
	}
	// 그룹의 여관 상태 변경
	InnState = ECustomerInnState::Kitchen;
}

void UInnCustomerGroup::FinishedEating()
{
	UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	
	MealState = ECustomerMealState::FinishedEating;
	// 사용하던 좌석 반납
	InnSystem->ReturnGroupSeats(GroupID); 
	
	// 식사 완료 이벤트 통보
	for (AInnCustomer* Member : MemberActors)
	{
		Member->OnGroupMealFinished(true);
	}

	// 숙박 여부 결정 프로세스로 이동
	DecideToLodge();
	OnGroupChanged.Broadcast(GroupID);
}

void UInnCustomerGroup::DecideToLodge()
{
	if (InnState == ECustomerInnState::Entrance) return;

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	UTimeSubsystem* TimeSystem = GameInstance->GetSubsystem<UTimeSubsystem>();
	UInnManagerSubsystem* InnSystem = GameInstance->GetSubsystem<UInnManagerSubsystem>();
	if (!TimeSystem || !InnSystem) return;
    
	// 1. 먼저 그룹 전체의 숙박 여부를 단 한 번만 판별합니다 (루프 외부에서 처리)
	int32 RandomValue = FMath::RandRange(1, 100);
	bool bHasEmptyRoom = InnSystem->HasEmptyRoom();
	bool bWantToStay = bHasEmptyRoom && (RandomValue <= 50 + Satisfaction) && (TimeSystem->GetCurrentHour() >= 9);
	bool bSuccessfullyCheckedIn = false;

	if (bWantToStay)
	{
		// 빈 방이 있고 조건이 맞으면 체크인 시도
		bSuccessfullyCheckedIn = InnSystem->CheckInCustomer(GroupID);
	}

	// 2. 판별된 최종 상태를 기반으로 모든 멤버의 상태를 업데이트합니다.
	if (bSuccessfullyCheckedIn)
	{
		InnState = ECustomerInnState::Room;
		for (AInnCustomer* Member : MemberActors)
		{
			if (IsValid(Member)) Member->OnGroupDecidedToStay(true);
		}
	}
	else
	{
		InnState = ECustomerInnState::Exit;
		for (AInnCustomer* Member : MemberActors)
		{
			if (IsValid(Member)) Member->OnGroupDecidedToStay(false);
		}
	}
}

// ============================================================================
// 객실 서비스 및 방 관리 (Room Services)
// ============================================================================
void UInnCustomerGroup::EnterRoom()
{
	// 방에 들어갈 때 액터 데이터를 스냅샷으로 저장하고 액터는 월드에서 제거
	for (AInnCustomer* Member : MemberActors)
	{
		if (IsValid(Member))
		{
			FCustomerSnapshot Snapshot = MakeCustomerSnapshot(Member);
			MemberSnapshots.Add(Snapshot);
			Member->Destroy();
		}
	}
	// 기존 포인터 배열 비우기
	MemberActors.Empty(); 
}

void UInnCustomerGroup::RespawnMembers()
{
	UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	if (!InnSystem) return;

	// 리스폰 전 이전 멤버 포인터 데이터 정리
	MemberActors.Empty();

	// 저장된 스냅샷을 기반으로 방 입구에 멤버들을 재소환
	for (FCustomerSnapshot& MemberSnapshot : MemberSnapshots)
	{
		FCustomerData CustomerData = UDonInnLibrary::GetCustomerAssetData(this, MemberSnapshot.Type);
        
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
			MemberActors.Add(Customer);
		}
	}
}

void UInnCustomerGroup::PayCost()
{
	if (AssignedRoomNumber == -1) return;
    
	UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
	ADonInnGameMode* InnGameMode = Cast<ADonInnGameMode>(UGameplayStatics::GetGameMode(this));
	if (!InnSystem || !InnGameMode) return;

	// 객실 기본가, 옵션가, 등급 및 만족도를 반영한 최종 정산 요금 계산
	FRoomInfo RoomInfo = InnSystem->GetRoomInfo(AssignedRoomNumber);
	int32 Cost = (RoomInfo.BasePrice + RoomInfo.ExtraPrice) * RoomInfo.Grade + (100 * Satisfaction);
    
	// 게임모드 매출에 추가
	InnGameMode->AddToRevenue(Cost);
}

void UInnCustomerGroup::CheckOut()
{
	RespawnMembers(); // 방 내부에서 다시 월드로 액터 소환
	PayCost();        // 정산
    
	InnState = ECustomerInnState::Exit;
}

// ============================================================================
// 룸서비스 상세 타이머 및 미니게임 (Room Service Internals)
// ============================================================================

void UInnCustomerGroup::SetRoomServiceTimer()
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &UInnCustomerGroup::RequestRoomService);
    
	// 3초에서 7초 사이의 랜덤한 대기 시간 후 룸서비스 요청
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
	// 제한시간 초과 시 타이머 종료 및 델리게이트 정리
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