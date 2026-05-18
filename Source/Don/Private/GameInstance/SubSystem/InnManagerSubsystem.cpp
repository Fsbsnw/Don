// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "DonInnGameMode.h"
#include "GameInstance/SubSystem/TimeSubsystem.h"
#include "Inn/DonInnLibrary.h"
#include "Inn/Actor/InnSeat.h"
#include "Inn/Data/CustomerAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Inn/Object/InnCustomerGroup.h"

// ============================================================================
// 시스템 초기화 및 데이터 세팅 (Initialization)
// ============================================================================

void UInnManagerSubsystem::InitInformation()
{
    // 1. 월드 내 고정 태그 액터들의 위치 정보 캐싱
    CacheWorldLocations();

    // 2. 좌석(식당 자리) 데이터 구조 빌드
    BuildSeatGroups();
    
    // 3. 최초 1회만 실행되어야 하는 서브시스템 및 데이터 정의
    if (!bInitialized)
    {
        InitializeSubsystemConnections();
        InitializeNamingPools();
    }

    // 4. 여관 레벨(고객 생성) 시작
    StartInnLevel();
    
    bInitialized = true;
}

void UInnManagerSubsystem::CacheWorldLocations()
{
    TArray<AActor*> FoundActors;

    // Exit 위치 캐싱
    UGameplayStatics::GetAllActorsWithTag(this, FName("Exit"), FoundActors);
    if (FoundActors.Num() > 0) ExitLocation = FoundActors[0]->GetActorLocation();

    // InnEntrance 위치 캐싱
    UGameplayStatics::GetAllActorsWithTag(this, FName("InnEntrance"), FoundActors);
    if (FoundActors.Num() > 0) InnEntranceLocation = FoundActors[0]->GetActorLocation();

    // RoomEntrance 위치 캐싱
    UGameplayStatics::GetAllActorsWithTag(this, FName("RoomEntrance"), FoundActors);
    if (FoundActors.Num() > 0) RoomEntranceLocation = FoundActors[0]->GetActorLocation();

    // CustomerSpawnLocation 위치 캐싱
    UGameplayStatics::GetAllActorsWithTag(this, FName("CustomerSpawnLocation"), FoundActors);
    if (FoundActors.Num() > 0) CustomerSpawnLocation = FoundActors[0]->GetActorLocation();
}

void UInnManagerSubsystem::BuildSeatGroups()
{
    TArray<AActor*> SeatActors;
    UGameplayStatics::GetAllActorsOfClass(this, AInnSeat::StaticClass(), SeatActors);

    SeatGroups.Empty();
    SeatGroups.SetNum(MaxSeatGroupSize);
    
    for (AActor* Actor : SeatActors)
    {
        AInnSeat* Seat = Cast<AInnSeat>(Actor);
        if (Seat && SeatGroups.IsValidIndex(Seat->SeatGroup))
        {
            SeatGroups[Seat->SeatGroup].Seats.Add(Seat);
            SeatGroups[Seat->SeatGroup].GroupSize++;
        }
    }  
}

void UInnManagerSubsystem::InitializeSubsystemConnections()
{
    // 게임모드로부터 객실 데이터 동기화
    if (ADonInnGameMode* InnGameMode = Cast<ADonInnGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        RoomInfos = InnGameMode->RoomInfos;
        RoomCapacity = InnGameMode->RoomInfos.Num();
    }

    // 시간 시스템 구독 (아침 체크아웃 연동)
    if (UGameInstance* GI = GetGameInstance())
    {
        if (UTimeSubsystem* TimeSubsystem = GI->GetSubsystem<UTimeSubsystem>())
        {
            TimeSubsystem->OnMorning.AddUObject(this, &UInnManagerSubsystem::MorningCheckOut);
        }  
    }
}

void UInnManagerSubsystem::InitializeNamingPools()
{
    InnGroupFirstNames = { TEXT("미숙한"), TEXT("평범한"), TEXT("숙련된") };
    InnGroupSecondNames = { TEXT("지방의"), TEXT("대도시"), TEXT("귀족") };
    InnGroupThirdNames = { TEXT("사냥꾼"), TEXT("탐험가"), TEXT("상인") };
}

void UInnManagerSubsystem::ResetData()
{
    bInitialized = false;
    InnGroups.Empty();
    LodgerGroups.Empty();
    
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(SpawnTimerHandle);
    }
}


// ============================================================================
// 스케줄러 및 스폰 로직
// ============================================================================

void UInnManagerSubsystem::StartInnLevel()
{
    ScheduleNextSpawn();
}

void UInnManagerSubsystem::ScheduleNextSpawn()
{
    // 현재 스폰을 처리하고 다음 스폰 시간 무작위 예약
    SpawnAndRegisterCustomerGroup(CustomerSpawnLocation, FMath::RandRange(1, 4));
    
    float RandomDelay = FMath::FRandRange(15.0f, 20.f);
    GetWorld()->GetTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &UInnManagerSubsystem::ScheduleNextSpawn,
        RandomDelay,
        false
    );
}

void UInnManagerSubsystem::SpawnAndRegisterCustomerGroup(FVector SpawnLocation, int32 Size)
{
    int32 CurrentGroupID = NextGroupID;
    FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);

    // 새 고객 그룹 객체(UObject) 할당
    UInnCustomerGroup* NewGroup = NewObject<UInnCustomerGroup>(this);
    if (!NewGroup) return;

    NewGroup->GroupID = CurrentGroupID;
    NewGroup->GroupName = NewCustomerName();
    NewGroup->OnGroupChanged.AddUObject(this, &UInnManagerSubsystem::UpdateInnGroup);
    NewGroup->OnGroupRoomServiceRequested.AddUObject(this, &UInnManagerSubsystem::OnRoomServiceReceived);
    
    // 그룹 크기만큼 멤버 액터 스폰
    for (int32 i = 1; i <= Size; ++i)
    {
        FVector RandomOffset(FMath::FRandRange(-100.f, 100.f), FMath::FRandRange(-100.f, 100.f), 0.f);
        FTransform RandomTransform(SpawnTransform.GetRotation(), SpawnTransform.GetLocation() + RandomOffset);

        ECustomerType RandomType = static_cast<ECustomerType>(FMath::RandRange(0, 2));
        FCustomerData CustomerData = UDonInnLibrary::GetCustomerAssetData(this, RandomType);
       
        AInnCustomer* Member = GetWorld()->SpawnActor<AInnCustomer>(CustomerData.CustomerClass, RandomTransform);
        if (Member)
        {
            Member->SetLevel(FMath::RandRange(1, 10));
            Member->SetGroupID(CurrentGroupID);
            Member->CustomerType = RandomType;
            Member->Portrait = CustomerData.CustomerPortrait;
            
            // 멤버 액터의 상태 변화를 그룹 오브젝트가 수신하도록 연결
            Member->OnCustomerChanged.AddUObject(NewGroup, &UInnCustomerGroup::OnMemberNotified);
            NewGroup->MemberActors.Add(Member);
        }
    }

    if (NewGroup->MemberActors.Num() > 0)
    {
        NewGroup->Portrait = NewGroup->MemberActors[0]->Portrait;
    }

    InnGroups.Add(CurrentGroupID, NewGroup);
    
    ++NextGroupID;
    ++TodayCustomerGroupCount;
    
    // UI 갱신 브로드캐스트
    OnInnGroupChanged.Broadcast(GetInnGroupsArray());
}


// ============================================================================
// 식당 테이블 관리 로직
// ============================================================================

bool UInnManagerSubsystem::FindAndOccupyEmptySeat(int32 GroupID)
{
    UInnCustomerGroup** GroupPtr = InnGroups.Find(GroupID);
    if (!GroupPtr || !*GroupPtr) return false;

    UInnCustomerGroup* Group = *GroupPtr;
    const int32 GroupSize = Group->MemberActors.Num();
    
    for (int32 i = 0; i < SeatGroups.Num(); ++i)
    {
        FSeatGroup& SeatGroup = SeatGroups[i];
        const int32 SeatSize = SeatGroup.Seats.Num();
        
        // 빈 자리 검사 조건 최적화 및 안전성 확보
        if (SeatGroup.AssignedGroupID != -1 || SeatSize < GroupSize) continue;
        if (SeatSize > 0 && !SeatGroup.Seats[0]->bIsAvailable) continue;

        // 적합한 테이블 발견 시 그룹 및 멤버 매핑
        Group->AssignedTable = i;
        SeatGroup.AssignedGroupID = GroupID;
        
        TArray<AInnCustomer*>& Members = Group->MemberActors;
        for (AInnSeat* Seat : SeatGroup.Seats)
        {
            for (AInnCustomer* Member : Members)
            {
                if (!IsValid(Member) || Member->Seat != nullptr || Seat->GetIsOccupied()) continue;

                Seat->SetIsOccupied(true);
                Member->ReserveSeat(Seat);
                break; // 해당 좌석에 한 명이 앉았으므로 다음 좌석으로 이동
            }
        }
        return true;
    }
    return false;
}

void UInnManagerSubsystem::ReturnGroupSeats(int32 GroupID)
{
    UInnCustomerGroup** GroupPtr = InnGroups.Find(GroupID);
    if (!GroupPtr || !*GroupPtr) return;

    int32 TableIndex = (*GroupPtr)->AssignedTable;
    if (SeatGroups.IsValidIndex(TableIndex))
    {
        SeatGroups[TableIndex].AssignedGroupID = -1;
    }
}


// ============================================================================
// 객실 관리 및 정산
// ============================================================================

bool UInnManagerSubsystem::AssignCustomerToRoom(int32 GroupID, int32 RoomIndex)
{
    UInnCustomerGroup* Group = GetGroupInfo(GroupID);
    if (!Group || Group->AssignedRoomNumber != -1 || !RoomInfos.IsValidIndex(RoomIndex)) return false;

    Group->InitRoomService();
    Group->AssignedRoomNumber = RoomIndex;
    RoomInfos[RoomIndex].GroupID = GroupID;
    RoomInfos[RoomIndex].bIsOccupied = true;
    Group->SetRoomServiceTimer();
    return true;
}

bool UInnManagerSubsystem::CheckInCustomer(int32 GroupID)
{
    if (!InnGroups.Contains(GroupID)) return false;
    LodgerGroups.Add(GroupID);
    return true;
}

void UInnManagerSubsystem::MorningCheckOut()
{
    ADonInnGameMode* InnGameMode = Cast<ADonInnGameMode>(UGameplayStatics::GetGameMode(this));

    // 안전하게 복사본을 만들어 순회 및 정산 후 한꺼번에 비웁니다.
    TArray<int32> CurrentLodgers = LodgerGroups.Array();

    for (int32 LodgerGroup : CurrentLodgers)
    {
        UInnCustomerGroup** GroupPtr = InnGroups.Find(LodgerGroup);
        if (!GroupPtr || !*GroupPtr) continue;

        UInnCustomerGroup* Group = *GroupPtr;

        // 1. 방 정보 초기화
        int32 RoomNumber = Group->AssignedRoomNumber;
        if (RoomInfos.IsValidIndex(RoomNumber))
        {
            RoomInfos[RoomNumber].bIsOccupied = false;
            RoomInfos[RoomNumber].GroupID = -1;
        }

        // 2. 의심도 페널티 계산 (생존 멤버 비율에 따른 수치 적용)
        int32 Suspicion = (Group->MemberActors.Num() - Group->DeadMemberCount) * 25;
        if (InnGameMode)
        {
            InnGameMode->AddToSuspicion(Suspicion);
        }

        // 3. 체크아웃 및 데이터 제거
        Group->CheckOut();
        InnGroups.Remove(LodgerGroup);
    }

    LodgerGroups.Empty();
    OnInnGroupChanged.Broadcast(GetInnGroupsArray());
}

void UInnManagerSubsystem::CloseInnAtMidnight()
{
    GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

    // 자정 객실 요금 정산
    int32 TodayRoomRevenue = 0;
    for (int32 LodgerGroup : LodgerGroups)
    {
        UInnCustomerGroup** GroupPtr = InnGroups.Find(LodgerGroup);
        if (!GroupPtr || !*GroupPtr) continue;

        int32 RoomNumber = (*GroupPtr)->AssignedRoomNumber;
        if (RoomInfos.IsValidIndex(RoomNumber))
        {
            FRoomInfo& AssignedRoom = RoomInfos[RoomNumber];
            TodayRoomRevenue += AssignedRoom.BasePrice + (AssignedRoom.ExtraPrice * (*GroupPtr)->Satisfaction);
        }
    }
    RoomServiceRevenue = TodayRoomRevenue;

    if (ADonInnGameMode* InnGameMode = Cast<ADonInnGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        InnGameMode->AddToRevenue(RoomServiceRevenue);
    }
    
    // 영업 종료 시 방에 있지 않은(식사 대기 중 등) 모든 일반 고객 그룹 강제 퇴장 처리
    TArray<int32> KeysToRemove;
    for (const auto& [Key, Value] : InnGroups)
    {
        if (Value && Value->InnState == ECustomerInnState::Room && Value->AssignedRoomNumber != -1) continue;
        KeysToRemove.AddUnique(Key);
    }

    for (int32 Key : KeysToRemove)
    {
        RemoveGroup(Key);
    }
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

    if (InnGroups.Contains(GroupID) && InnGroups[GroupID])
    {
        InnGroups[GroupID]->ExitInn();
    }
    
    InnGroups.Remove(GroupID);
    LodgerGroups.Remove(GroupID);
    OnInnGroupChanged.Broadcast(GetInnGroupsArray());
}


// ============================================================================
// Getter 및 유틸리티 함수
// ============================================================================

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

TArray<UInnCustomerGroup*> UInnManagerSubsystem::GetInnGroupsArray() const
{
    TArray<UInnCustomerGroup*> GroupsForUI;
    for (const auto& [Key, Value] : InnGroups)
    {
        if (Value) GroupsForUI.Add(Value);
    }

    // 상태 값을 기준으로 안정 정렬(Stable Sort)
    GroupsForUI.StableSort([](const UInnCustomerGroup& A, const UInnCustomerGroup& B)
    {
        return A.InnState > B.InnState;
    });
    
    return GroupsForUI;
}

TArray<UInnCustomerGroup*> UInnManagerSubsystem::GetLodgersArray() const
{
    TArray<UInnCustomerGroup*> GroupsForUI;
    for (const auto& [Key, Value] : InnGroups)
    {
        if (Value && Value->InnState == ECustomerInnState::Room)
        {
            GroupsForUI.AddUnique(Value);        
        }
    }
    return GroupsForUI;
}

ECustomerInnState UInnManagerSubsystem::GetGroupInnState(int32 GroupID)
{
    if (!InnGroups.Contains(GroupID) || !InnGroups[GroupID]) return ECustomerInnState::Exit;
    return InnGroups[GroupID]->InnState;
}

int32 UInnManagerSubsystem::GetGroupSize(int32 GroupID)
{
    if (!InnGroups.Contains(GroupID) || !InnGroups[GroupID]) return 0;
    return InnGroups[GroupID]->MemberActors.Num(); 
}

void UInnManagerSubsystem::SetGroupTable(int32 GroupID, int32 TableNumber)
{
    if (!InnGroups.Contains(GroupID) || !InnGroups[GroupID]) return;
    InnGroups[GroupID]->AssignedTable = TableNumber;
}

FInnCustomerGroupSnapshot UInnManagerSubsystem::GetLodgerInfo(int32 RoomNumber)
{
    if (!RoomInfos.IsValidIndex(RoomNumber)) return FInnCustomerGroupSnapshot();

    int32 GroupID = RoomInfos[RoomNumber].GroupID;
    if (GroupID == -1 || !InnGroups.Contains(GroupID) || !InnGroups[GroupID]) return FInnCustomerGroupSnapshot();

    UInnCustomerGroup* Group = InnGroups[GroupID];
    
    FInnCustomerGroupSnapshot Lodger;
    Lodger.GroupID = GroupID;
    Lodger.GroupPortrait = Group->Portrait;
    Lodger.GroupSize = Group->MemberActors.Num();
    Lodger.SatisfactionScore = Group->Satisfaction;
    Lodger.RoomService = Group->GroupRoomService;
    
    return Lodger;
}

bool UInnManagerSubsystem::HasEmptyRoom() const
{
    int32 AssignedRoomCount = 0;
    for (int32 LodgerGroup : LodgerGroups)
    {
        const UInnCustomerGroup* const* GroupPtr = InnGroups.Find(LodgerGroup);
        if (GroupPtr && *GroupPtr && (*GroupPtr)->AssignedRoomNumber != -1)
        {
            ++AssignedRoomCount;
        }
    }
    return AssignedRoomCount < RoomCapacity;
}

FString UInnManagerSubsystem::NewCustomerName()
{
    if (InnGroupFirstNames.Num() == 0 || InnGroupSecondNames.Num() == 0 || InnGroupThirdNames.Num() == 0)
    {
        return TEXT("익명의 모험가");
    }

    FString NewName;
    NewName += InnGroupFirstNames[FMath::RandRange(0, InnGroupFirstNames.Num() - 1)] + TEXT(" ");
    NewName += InnGroupSecondNames[FMath::RandRange(0, InnGroupSecondNames.Num() - 1)] + TEXT(" ");
    NewName += InnGroupThirdNames[FMath::RandRange(0, InnGroupThirdNames.Num() - 1)];

    return NewName;
}

UInnCustomerGroup* UInnManagerSubsystem::GetGroupInfo(int32 GroupID) const
{
    UInnCustomerGroup* const* GroupPtr = InnGroups.Find(GroupID);
    return (GroupPtr) ? *GroupPtr : nullptr;
}