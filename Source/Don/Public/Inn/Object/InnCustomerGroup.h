// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/RoomServiceAsset.h"
#include "Inn/Character/InnCustomer.h"
#include "InnCustomerGroup.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGroupChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoomServiceChanged, float);

struct FCustomer;
class AInnCustomer;



// =========================================================
// 저장 및 복구를 위한 고객 데이터 스냅샷 구조체
// =========================================================
USTRUCT()
struct FCustomerSnapshot
{
	GENERATED_BODY()

	UPROPERTY()
	ECustomerType Type = ECustomerType::First;

	UPROPERTY()
	int32 Level = 1;
};

// =========================================================
// 여관에 방문한 고객 그룹의 상태와 서비스를 관리하는 클래스
// =========================================================
UCLASS(BlueprintType)
class DON_API UInnCustomerGroup : public UObject
{
	GENERATED_BODY()

public:
// =========================================================
// 그룹 공통 데이터
// =========================================================

	UPROPERTY()
	UTexture2D* Portrait;
	
	UPROPERTY(BlueprintReadOnly)
	int32 GroupID = -1;
	
	UPROPERTY(BlueprintReadOnly)
	FString GroupName;

	/* 여관에서의 전체 만족도 */
	UPROPERTY(BlueprintReadOnly)
	int32 Satisfaction = 0;
	
	UPROPERTY(BlueprintReadOnly)
	ECustomerInnState InnState = ECustomerInnState::Entrance;

// =========================================================
// 델리게이트
// =========================================================
	
	FOnGroupChanged OnGroupChanged;
	FOnGroupChanged OnGroupRoomServiceRequested;
	FOnRoomServiceChanged OnRoomServiceChanged;
	
// =========================================================
// 멤버 관리
// =========================================================
	
	UPROPERTY()
	TArray<AInnCustomer*> MemberActors;

	/* 객실 입장 시에 갖고 있는 스냅샷 데이터 */
	UPROPERTY()
	TArray<FCustomerSnapshot> MemberSnapshots;

	/* 던전에서 죽은 멤버 */
	int32 DeadMemberCount = 0;

// =========================================================
// 식당 서비스 데이터
// =========================================================

	/* 그룹의 식사 상태 */
	UPROPERTY(BlueprintReadOnly)
	ECustomerMealState MealState = ECustomerMealState::WaitingForFood;

	/* 그룹이 할당 받은 테이블 번호 */
	UPROPERTY(BlueprintReadOnly)
	int32 AssignedTable = -1;

// =========================================================
// 객실 서비스 데이터
// =========================================================

	/* 그룹이 할당 받은 객실 번호 */
	UPROPERTY(BlueprintReadOnly)
	int32 AssignedRoomNumber = -1;

	/* 객실 서비스의 시간 제한 */
	float MaxRoomServiceTimeLimit;
	/* 요청할 객실 서비스 정보 */
	FRoomServiceOrder GroupRoomService;
	/* 객실 서비스 시간 제한 카운팅 타이머 */
	FTimerHandle RoomServiceTimeLimitHandle;

public:
// =========================================================
// 공통 및 관리 함수
// =========================================================

	/* 만족도를 증가/감소 */
	void AddToSatisfaction(int32 InSatisfaction);
	
	/* 멤버 액터로부터 스냅샷 데이터 생성 */
	FCustomerSnapshot MakeCustomerSnapshot(AInnCustomer* Customer);

	/* 멤버들을 다시 스폰 */
	void RespawnMembers();
	
	/* 여관에서 완전히 퇴장 */
	void ExitInn();

// =========================================================
// 식당 서비스 함수
// =========================================================
	
	/* 식당 자리를 요청 */
	void RequestSeats();

	/* 멤버에게 알림 이벤트가 도달했을 때의 처리 */
	void OnMemberNotified(ECustomerNotify Notify);

	/* 식사를 완료했을 때 호출 */
	void FinishedEating();

	/* 식사 후 숙박할지 여부를 결정 */
	void DecideToLodge();
	
// =========================================================
// 객실 서비스 함수
// =========================================================
	
	/* 배정된 방에 입장 */
	void EnterRoom();

	/* 룸 서비스 타이머를 설정 */
	void SetRoomServiceTimer();

	/* 룸 서비스를 초기화 */
	void InitRoomService();

	/* 룸 서비스를 요청 */
	void RequestRoomService();

	/* 룸 서비스 제한 시간을 갱신(Tick) */
	void TickRoomService();

	/* 룸 서비스 미니게임의 성공 여부를 설정 */
	void SetMiniGameSuccess(bool bIsSuccess);

	/* 비용을 지불 */
	void PayCost();

	/* 객실 체크아웃 */
	void CheckOut();
	
private:
// =========================================================
// 내부 카운터 변수
// =========================================================
	int32 FinishedEatingCount = 0;
	int32 EnteredMemberCount = 0;
	int32 EnteredRoomMemberCount = 0;
	int32 ExitMemberCount = 0;
	
};