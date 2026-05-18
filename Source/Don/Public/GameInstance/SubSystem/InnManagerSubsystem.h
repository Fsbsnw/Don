// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/RoomServiceAsset.h"
#include "Inn/Character/InnCustomer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InnManagerSubsystem.generated.h"


struct FCustomerSnapshot;
class UInnCustomerGroup;
struct FSeatGroup;


USTRUCT(BlueprintType)
struct FRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsOccupied = false;

	UPROPERTY(BlueprintReadOnly)
	int32 GroupID = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Grade = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Size = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 BasePrice = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ExtraPrice = 0;
};

USTRUCT(BlueprintType)
struct FCustomer
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 GroupID = 0;

	UPROPERTY()
	TWeakObjectPtr<AInnCustomer> Customer;
	
	UPROPERTY(BlueprintReadOnly)
	ECustomerType Type = ECustomerType::First;
};

USTRUCT(BlueprintType)
struct FInnCustomerGroupSnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 GroupID = -1;
	
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* GroupPortrait = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	int32 GroupSize = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 SatisfactionScore = 0;

	UPROPERTY(BlueprintReadOnly)
	FRoomServiceOrder RoomService;
};

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnInnGroupChanged,
	const TArray<UInnCustomerGroup*>
);

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnInnGroupRequested,
	int32
);


// =========================================================
// 여관의 고객 스폰, 좌석 배정, 객실 체크인 및 정산을 총괄하는 서브 시스템
// =========================================================
UCLASS()
class DON_API UInnManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
// ============================================================================
// 델리게이트 및 이벤트
// ============================================================================
    
    FOnInnGroupChanged OnInnGroupChanged;
    FOnInnGroupRequested OnInnGroupRoomServiceReceived;

// ============================================================================
// 월드 주요 위치 정보 데이터
// ============================================================================
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inn|Locations")
    FVector ExitLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inn|Locations")
    FVector InnEntranceLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inn|Locations")
    FVector RoomEntranceLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inn|Locations")
    FVector CustomerSpawnLocation = FVector::ZeroVector;

// ============================================================================
// 객실 서비스 데이터
// ============================================================================
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inn|Room")
    TArray<FRoomInfo> RoomInfos;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inn|Room")
    int32 RoomCapacity = 10;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inn|Room")
    int32 RoomServiceRevenue = 0;

public:
// ============================================================================
// Getter 및 유틸리티
// ============================================================================
    
    /** 무작위 조합으로 새로운 고객 그룹 이름을 생성합니다. */
    FString NewCustomerName();

    /** 그룹 ID를 통해 고객 그룹의 포인터를 안전하게 가져옵니다. */
    UInnCustomerGroup* GetGroupInfo(int32 GroupID) const;

    /** 특정 그룹의 현재 여관 내부 상태(식당, 객실, 퇴장 등)를 구합니다. */
    ECustomerInnState GetGroupInnState(int32 GroupID);

    /** 특정 그룹의 멤버 멤버 수를 구합니다. */
    int32 GetGroupSize(int32 GroupID);

    /** UI 출력을 위해 현재 여관 내에 존재하는 모든 그룹 배열을 정렬하여 반환합니다. */
    TArray<UInnCustomerGroup*> GetInnGroupsArray() const;

    /** 현재 객실에 머물고 있는 숙박객 그룹 배열을 반환합니다. */
    TArray<UInnCustomerGroup*> GetLodgersArray() const;

// ============================================================================
// 식당 좌석 관리 함수
// ============================================================================
    
    /** 그룹의 크기에 맞는 빈 식당 테이블을 찾아 멤버들을 앉힙니다. */
    bool FindAndOccupyEmptySeat(int32 GroupID);

    /** 새로운 고객 그룹을 특정 위치에 스폰하고 시스템에 등록합니다. */
    void SpawnAndRegisterCustomerGroup(FVector SpawnLocation, int32 Size = 1);

    /** 강제로 특정 그룹의 식당 테이블 인덱스를 설정합니다. */
    void SetGroupTable(int32 GroupID, int32 TableNumber);

    /** 식사가 끝난 그룹의 좌석 선점을 해제합니다. */
    void ReturnGroupSeats(int32 GroupID);

    /** 그룹을 완전히 파괴하고 목록에서 제거합니다. */
    void RemoveGroup(int32 GroupID);

// ============================================================================
// 객실 및 룸서비스 함수 (Room & Lodger Services)
// ============================================================================
    
    /** 특정 객실의 방 정보를 가져옵니다. */
    FRoomInfo GetRoomInfo(int32 RoomNumber) { return RoomInfos.IsValidIndex(RoomNumber) ? RoomInfos[RoomNumber] : FRoomInfo(); }

    /** 특정 객실에 묵고 있는 투숙객들의 스냅샷 정보를 UI용으로 구성하여 반환합니다. */
    FInnCustomerGroupSnapshot GetLodgerInfo(int32 RoomNumber);

    /** 그룹을 숙박 리스트에 등록합니다. */
    bool CheckInCustomer(int32 GroupID);

    /** 지정한 방에 고객 그룹을 입실시키고 룸서비스 스케줄을 가동합니다. */
    bool AssignCustomerToRoom(int32 GroupID, int32 RoomIndex);

    /** 현재 여관 객실에 빈 방이 남아있는지 확인합니다. */
    bool HasEmptyRoom() const;

    /** 매일 아침 투숙객들을 정산하고 체크아웃 처리합니다. (OnMorning 바인딩) */
    void MorningCheckOut();

    /** 자정에 당일 매출을 확정하고 방에 없는 일반 고객들을 강제 퇴장시킵니다. */
    void CloseInnAtMidnight();

protected:
// ============================================================================
// 서브시스템 생명 주기 관리
// ============================================================================
    
    /** 월드 진입 시 필요한 데이터와 태그 액터들을 찾아 시스템을 초기화합니다. */
    UFUNCTION(BlueprintCallable, Category = "Inn|System")
    void InitInformation();

    /** 영업 종료 혹은 레벨 전환 시 서브시스템 데이터를 초기화합니다. */
    UFUNCTION(BlueprintCallable, Category = "Inn|System")
    void ResetData();

// ============================================================================
// 내부 이벤트 바인딩용 함수
// ============================================================================
    
    UFUNCTION()
    void UpdateInnGroup(int32 GroupID);

    UFUNCTION()
    void OnRoomServiceReceived(int32 GroupID);

private:
    // ============================================================================
    // InitInformation 내부에서 세분화된 초기화 서브 프로세스
    // ============================================================================
    
    /** 월드 내 고정 태그 액터(Exit, Entrance 등)들의 위치를 캐싱합니다. */
    void CacheWorldLocations();

    /** 월드의 좌석 액터들을 탐색하여 테이블 그룹 데이터 구조를 빌드합니다. */
    void BuildSeatGroups();

    /** 게임모드 연동 및 시간 서브시스템 델리게이트를 바인딩합니다. */
    void InitializeSubsystemConnections();

    /** 고객 이름 생성에 사용될 언어 풀 배열을 채웁니다. */
    void InitializeNamingPools();

    // ============================================================================
    // 고객 스폰 및 내부 관리 변수
    // ============================================================================
    
    /** 여관 레벨이 시작될 때 최초 스폰 예약을 시작합니다. */
    void StartInnLevel();

    /** 랜덤 딜레이를 계산하여 주기적으로 다음 고객이 오도록 타이머를 돌립니다. */
    void ScheduleNextSpawn();

    UPROPERTY()
    TMap<int32, UInnCustomerGroup*> InnGroups;
    
    UPROPERTY()
    TSet<int32> LodgerGroups;

    UPROPERTY()
    TArray<FSeatGroup> SeatGroups;
    
    UPROPERTY()
    TArray<FString> InnGroupFirstNames;
    UPROPERTY()
    TArray<FString> InnGroupSecondNames;
    UPROPERTY()
    TArray<FString> InnGroupThirdNames;

    FTimerHandle SpawnTimerHandle;

    int32 TodayCustomerGroupCount = 0;
    int32 NextGroupID = 1;
    int32 MaxSeatGroupSize = 12;

    bool bInitialized = false;
};