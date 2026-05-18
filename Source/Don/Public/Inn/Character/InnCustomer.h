// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CuisineAsset.h"
#include "GameFramework/Character.h"
#include "InnCustomer.generated.h"

struct FPathFollowingResult;
class UBehaviorTree;
class AInnSeat;
struct FKitchenOrder;

UENUM(BlueprintType)
enum class ECustomerNotify : uint8
{
	EnterInn			UMETA(DisplayName = "EnterInn"),  
	FinishedEating      UMETA(DisplayName = "FinishedEating"),
	EnterRoom			UMETA(DisplayName = "EnterRoom"),
	ExitInn				UMETA(DisplayName = "ExitInn"),
};

UENUM(BlueprintType)
enum class ECustomerType : uint8
{
	First		UMETA(DisplayName = "First"),  
	Second      UMETA(DisplayName = "Second"),
	Third       UMETA(DisplayName = "Third"),           
};

UENUM(BlueprintType)
enum class ECustomerInnState : uint8
{
	Entrance	 UMETA(DisplayName = "Entrance"),
	Kitchen      UMETA(DisplayName = "Kitchen"),  
	Room         UMETA(DisplayName = "Room"),
	Exit         UMETA(DisplayName = "Exit"),           
};

UENUM(BlueprintType)
enum class ECustomerMealState : uint8
{
	WaitingForFood      UMETA(DisplayName = "Waiting For Food"),  
	Eating              UMETA(DisplayName = "Eating"),           
	FinishedEating      UMETA(DisplayName = "Finished Eating")   
};

UENUM(BlueprintType)
enum class ECustomerSeatState : uint8
{
	Idle				UMETA(DisplayName = "Idle"),  
	MoveToSeat          UMETA(DisplayName = "Move To Seat"),           
	Sit					UMETA(DisplayName = "Sit")   
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCustomerChanged, ECustomerNotify);

// ============================================================================
// 여관에 방문하는 개별 손님 객체의 상태와 AI 연동 로직을 담당하는 액터 클래스
// ============================================================================
UCLASS()
class DON_API AInnCustomer : public ACharacter
{
	GENERATED_BODY()
public:
// ============================================================================
// 델리게이트
// ============================================================================
    
    FOnCustomerChanged OnCustomerChanged;

public:
// ============================================================================
// Getter, Setter 함수
// ============================================================================
    
    FGuid GetID() const { return ID; }
    int32 GetLevel() const { return Level; }
    int32 GetGroupID() const { return GroupID; }
    int32 GetSatisfaction() const { return Satisfaction; }
    
    void SetGroupID(int32 NewID) { GroupID = NewID; }
    void SetLevel(int32 NewLevel) { Level = NewLevel; }
    void SetInnState(ECustomerInnState State) { InnState = State; }

    UFUNCTION(BlueprintCallable, Category = "Inn|Customer|Status")
    FORCEINLINE ECustomerType GetType() const { return CustomerType; }

    UFUNCTION(BlueprintCallable, Category = "Inn|Customer|Status")
    FORCEINLINE ECustomerMealState GetMealState() const { return MealState; }

    UFUNCTION(BlueprintCallable, Category = "Inn|Customer|Status")
    FORCEINLINE ECustomerInnState GetInnState() const { return InnState; }

    UFUNCTION(BlueprintCallable, Category = "Inn|Customer|Status")
    FORCEINLINE ECustomerSeatState GetSeatState() const { return SeatState; }

// ============================================================================
// 여관 입장 및 상태 제어 함수
// ============================================================================

    /** 매니저 시스템에 여관 입장 완료 알림을 보냅니다. */
    void EnterInn();

    /** 매니저 시스템에 여관 완전히 퇴장했음을 알립니다. */
    void ExitInn();

    /** 목표 상태에 따른 월드 내 이동 목적지 좌표를 설정하고 계산합니다. */
    void SetDestination(const ECustomerInnState& Destination);

    /** AI MoveTo 등에서 참조할 현재 목적지 벡터 좌표를 반환합니다. */
    FVector GetDestination() const { return NextDestination; }

// ============================================================================
// 식당 및 요리 주문 함수 
// ============================================================================
    
    /** AI 블랙보드에 빈 좌석 발견 유무 상태를 업데이트합니다. */
    void OnSeatAssigned(bool State);
    
    /** 게임 인스턴스를 통해 랜덤한 요리 주문서를 생성합니다. */
    void CreateFoodOrder();

    /** 생성된 주문서를 주방 오더 서브시스템 대기열에 등록합니다. */
    void OrderFood();

    /** 요리가 완성되었을 때 음식을 테이블 위에 스폰하고 식사 상태로 전이합니다. */
    void ReceiveFood();
    
    /** 식사를 마치고 개인 완료 알림을 그룹에 보냅니다. */
    void FinishMeal();

    /** 그룹 내 전원이 식사를 마쳤을 때 호출되며, 좌석과 남은 음식을 정리합니다. */
    void OnGroupMealFinished(bool State);

    /** 식사가 끝난 후 숙박 여부가 결정되었을 때 AI 블랙보드 값을 갱신합니다. */
    void OnGroupDecidedToStay(bool State);

    /** 주문한 음식의 가격을 가져옵니다. */
    int32 GetFoodPrice() const;

// ============================================================================
// 좌석 상호작용 함수
// ============================================================================

    /** 매니저에 의해 선점된 빈 좌석을 캐릭터에게 할당합니다. */
    void ReserveSeat(AInnSeat* NewSeat);

    /** 할당된 의자의 SitPoint 트랜스폼으로 캐릭터 위치를 고정하고 앉는 상태로 변경합니다. */
    void SitOnSeat();

// ============================================================================
// 객실 서비스 함수 
// ============================================================================
    
    /** 숙박을 위해 방으로 입장함을 알리고 스냅샷을 생성할 수 있도록 유도합니다. */
    void EnterRoom();

protected:
    // ============================================================================
    // 수명 주기 및 블루프린트 이벤트 
    // ============================================================================

    AInnCustomer();

    virtual void BeginPlay() override;

    /** 음식을 수령했을 때 위젯이나 시각적 연출을 처리할 블루프린트 이벤트 */
    UFUNCTION(BlueprintImplementableEvent, Category = "Inn|Customer|UI")
    void ReceiveFoodUI();

public:
    // ============================================================================
    // 손님 여관 이용 정보
    // ============================================================================

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inn|Customer|Visual")
    UTexture2D* Portrait;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Inn|Customer|Seat")
    AInnSeat* Seat = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inn|Customer|Status")
    ECustomerType CustomerType = ECustomerType::First;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Inn|Customer|Kitchen")
    FKitchenOrder OrderedFood;

private:
    // ============================================================================
    // 내부 관리 변수
    // ============================================================================
    
    FGuid ID;

    int32 GroupID = 0;
    int32 Level = 1;
    int32 SelectedFood = 0;
    int32 Satisfaction = 0;

    ECustomerInnState InnState = ECustomerInnState::Entrance;
    ECustomerMealState MealState = ECustomerMealState::WaitingForFood;
    ECustomerSeatState SeatState = ECustomerSeatState::Idle;

    FVector NextDestination = FVector::ZeroVector;
    
    UPROPERTY(EditAnywhere, Category = "Inn|Customer|Config")
    float EatingTime = 10.f;

    /** 음식을 앞에 스폰할 때 캐릭터 중심점으로부터의 상대적 기준 좌표 */
    UPROPERTY(EditAnywhere, Category = "Inn|Customer|Config")
    FVector FoodSpawnOffset = FVector(60.f, 0.f, 162.f);

    /** 현재 테이블 앞에 스폰되어 놓여있는 음식 액터의 포인터 */
    UPROPERTY()
    AActor* ReceivedFoodActor;
};