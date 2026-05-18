// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Character/InnCustomer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/CuisineAsset.h"
#include "GameInstance/DonGameInstance.h"
#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"
#include "Inn/DonInnLibrary.h"
#include "Inn/Actor/InnSeat.h"
#include "Inventory/DonItemLibrary.h"

// ============================================================================
// 생성자 및 생명 주기
// ============================================================================

AInnCustomer::AInnCustomer()
{
    // 성능 최적화를 위해 Tick은 기본적으로 비활성화합니다.
    PrimaryActorTick.bCanEverTick = false;
}

void AInnCustomer::BeginPlay()
{
    Super::BeginPlay();

    // 고객 고유 식별자 생성
    ID = FGuid::NewGuid();
}


// ============================================================================
// 식당 및 요리 주문 로직
// ============================================================================

void AInnCustomer::CreateFoodOrder()
{
    UDonGameInstance* DGI = Cast<UDonGameInstance>(GetGameInstance());
    if (!DGI) return;

    // 게임 인스턴스로부터 무작위 메뉴 정보를 가져와 주문 서류를 생성합니다.
    OrderedFood = DGI->GetRandomCuisine();
    OrderedFood.OrderedCustomer = this;
}

void AInnCustomer::OrderFood()
{
    if (MealState == ECustomerMealState::FinishedEating) return;

    UKitchenOrderSubsystem* KitchenSystem = GetGameInstance()->GetSubsystem<UKitchenOrderSubsystem>();
    if (!KitchenSystem) return;

    CreateFoodOrder();
    KitchenSystem->EnqueueKitchenOrder(OrderedFood);   
}

void AInnCustomer::ReceiveFood()
{
    MealState = ECustomerMealState::Eating;

    // 좌석 배정 시 손님 앞 테이블 위치에 음식을 배치하기 위한 오프셋 계산
    const FVector FoodSpawnLocation = GetActorLocation()
        + (GetActorForwardVector() * FoodSpawnOffset.X)
        + (GetActorRightVector()   * FoodSpawnOffset.Y)
        + FVector(0.f, 0.f, FoodSpawnOffset.Z);
    
    FTransform SpawnTransform(FRotator::ZeroRotator, FoodSpawnLocation);
    if (AActor* FoodActor = GetWorld()->SpawnActor<AActor>(OrderedFood.FoodActor, SpawnTransform))
    {
        ReceivedFoodActor = FoodActor;
    }

    // UI 및 시각 효과 트리거
    ReceiveFoodUI();
}

void AInnCustomer::FinishMeal()
{    
    // 그룹 오브젝트에 개인 식사 완료를 알립니다. (모두 완료 시 다음 단계 이동용)
    OnCustomerChanged.Broadcast(ECustomerNotify::FinishedEating);
}

void AInnCustomer::OnGroupMealFinished(bool State)
{
    AAIController* AIController = Cast<AAIController>(GetController());
    if (!AIController) return;

    UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
    if (!BBComp) return;

    // AI 행동 트리 제어용 블랙보드 값 갱신
    BBComp->SetValueAsBool(FName("bAllFinishedEating"), State);

    MealState = ECustomerMealState::FinishedEating;
    SeatState = ECustomerSeatState::Idle;

    // 사용하던 좌석 반납 및 테이블 위 음식 액터 제거
    if (Seat)
    {
        Seat->SetIsOccupied(false);
        Seat = nullptr;
    }
    
    if (ReceivedFoodActor)
    {
        ReceivedFoodActor->Destroy();
        ReceivedFoodActor = nullptr;
    }
}

int32 AInnCustomer::GetFoodPrice() const
{
    return OrderedFood.Price;
}


// ============================================================================
// 그룹 상태 변화 알림 대리자
// ============================================================================

void AInnCustomer::OnSeatAssigned(bool State)
{
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BBComp = AIController->GetBlackboardComponent())
        {
            BBComp->SetValueAsBool(FName("bFoundEmptySeat"), State);
        }
    }
}

void AInnCustomer::OnGroupDecidedToStay(bool State)
{
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BBComp = AIController->GetBlackboardComponent())
        {
            BBComp->SetValueAsBool(FName("bUseRoom"), State);
        }
    }
}

void AInnCustomer::EnterInn()
{
    OnCustomerChanged.Broadcast(ECustomerNotify::EnterInn);
}

void AInnCustomer::EnterRoom()
{
    OnCustomerChanged.Broadcast(ECustomerNotify::EnterRoom);
}

void AInnCustomer::ExitInn()
{
    OnCustomerChanged.Broadcast(ECustomerNotify::ExitInn);
}


// ============================================================================
// 이동 경로 및 좌석 상호작용
// ============================================================================

void AInnCustomer::SetDestination(const ECustomerInnState& Destination)
{
    UInnManagerSubsystem* InnSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
    if (!InnSystem) return;

    switch (Destination)
    {
    case ECustomerInnState::Entrance:
        {
            InnState = ECustomerInnState::Entrance;
            FVector RandomOffset(FMath::FRandRange(-100.f, 100.f), FMath::FRandRange(-100.f, 100.f), 0.f);
            NextDestination = InnSystem->InnEntranceLocation + RandomOffset;
            break;
        }
       
    case ECustomerInnState::Kitchen:
        {
            if (Seat != nullptr)
            {
                InnState = ECustomerInnState::Kitchen;
                SeatState = ECustomerSeatState::MoveToSeat;
                NextDestination = Seat->GetActorLocation();          
            }
            break;
        }
       
    case ECustomerInnState::Room:
        {
            InnState = ECustomerInnState::Room;
            SeatState = ECustomerSeatState::Idle;
            NextDestination = InnSystem->RoomEntranceLocation;
            break;
        }
       
    case ECustomerInnState::Exit:
        {
            InnState = ECustomerInnState::Exit;
            NextDestination = InnSystem->ExitLocation;
            break;
        }
    }
}

void AInnCustomer::ReserveSeat(AInnSeat* NewSeat)
{
    if (MealState == ECustomerMealState::FinishedEating) return;

    Seat = NewSeat;
}

void AInnCustomer::SitOnSeat()
{
    if (Seat == nullptr || Seat->SitPoint == nullptr) return;
    
    SeatState = ECustomerSeatState::Sit;

    // 좌석 오브젝트가 가진 SitPoint 트랜스폼에 캐릭터 위치 고정
    SetActorLocation(Seat->SitPoint->GetComponentLocation());
    SetActorRotation(Seat->SitPoint->GetComponentRotation());
}