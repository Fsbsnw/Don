// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SubSystem/KitchenOrderSubsystem.h"

#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "Inn/Actor/InnChef.h"
#include "Inn/Character/InnCustomer.h"
#include "Inn/Object/InnCustomerGroup.h"
#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"

// ============================================================================
// 서브시스템 초기화 및 시작 
// ============================================================================

void UKitchenOrderSubsystem::BroadcastInitialValues()
{
	DonPlayerState = Cast<ADonPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	CompletedFoodOrders.Empty();
    
	// UI에 빈 대기열 정보 브로드캐스트
	OnKitchenOrderUpdated.Broadcast(KitchenOrderQueue);

	// 고용된 요리사 초기화 상태 반영
	for (AInnChef* Chef : Chefs)
	{
		if (IsValid(Chef))
		{
			Chef->Hired(Chef->bIsHired);
		}
	}
}

// ============================================================================
// 주문 대기열 및 요리사 배정
// ============================================================================

FKitchenOrder UKitchenOrderSubsystem::EnqueueKitchenOrder(FKitchenOrder& Order)
{
	Order.OrderID = FGuid::NewGuid();
	Order.RemainingTime = Order.CookingTime;
    
	KitchenOrderQueue.Add(Order);
    
	// 대기열에 들어온 즉시 놀고 있는 요리사가 있다면 배정
	AssignChef();
    
	OnKitchenOrderAdded.Broadcast(Order);
	return Order;
}

void UKitchenOrderSubsystem::AssignChef()
{
	for (FKitchenOrder& Order : KitchenOrderQueue)
	{
		// 이미 조리 중이거나 시간이 만료된 주문은 패스
		if (Order.bIsCooking || Order.RemainingTime <= 0.f) continue;
       
		// 대기 중인 요리사가 있다면 주문 할당 후 타이머 구동
		if (AInnChef* IdleChef = FindIdleChef())
		{
			Order.AssignedChef = IdleChef;
			IdleChef->StartOrder(Order);
          
			if (!GetWorld()->GetTimerManager().IsTimerActive(OrderTimerHandle))
			{
				GetWorld()->GetTimerManager().SetTimer(
					OrderTimerHandle,
					this,
					&UKitchenOrderSubsystem::UpdateKitchenOrders,
					TickTimer,
					true
				);
			}
		}
		else 
		{
			// 더 이상 가용한 요리사가 없으면 루프를 종료합니다.
			break;
		}
	}
}

AInnChef* UKitchenOrderSubsystem::FindIdleChef()
{
	for (AInnChef* Chef : Chefs)
	{
		if (!Chef->IsCooking() && Chef->bIsHired) return Chef;
	}
	return nullptr;
}

FGuid UKitchenOrderSubsystem::FindNextQueuedOrderID() const
{
	for (const FKitchenOrder& Order : KitchenOrderQueue)
	{
		// 아직 조리가 시작되지 않은 대기열의 첫 주문 ID 반환
		if (!Order.bIsCooking)
		{
			return Order.OrderID;
		}
	}
	return FGuid();
}

// ============================================================================
// 주문 업데이트 및 완료 처리
// ============================================================================

void UKitchenOrderSubsystem::UpdateKitchenOrders()
{
    TArray<int32> IndexToRemove;

    // 1. 조리 중인 주문들의 남은 시간 차감 검사
    for (int32 i = 0; i < KitchenOrderQueue.Num(); i++)
    {
        FKitchenOrder& Order = KitchenOrderQueue[i];
        if (Order.bIsCooking && Order.RemainingTime > 0)
        {
            Order.RemainingTime -= TickTimer;

            if (Order.RemainingTime <= 0.f)
            {
                Order.bIsCooking = false;
                IndexToRemove.Add(i); // 이번 틱에 완료된 주문 인덱스 확보
            }
        }
    }

    // UI 대기열 정보 갱신 브로드캐스트
    OnKitchenOrderUpdated.Broadcast(KitchenOrderQueue);

    // 2. 완료된 주문 정산 처리 (배열 안전을 위해 역순 정렬 후 삭제)
    IndexToRemove.Sort(TGreater<int32>());

    for (int32 i : IndexToRemove)
    {
        FKitchenOrder CompletedOrder = KitchenOrderQueue[i];
        
        // 해당 요리사의 조리 상태 해제 및 보상 계산
        if (IsValid(CompletedOrder.AssignedChef))
        {
            CompletedOrder.AssignedChef->EndOrder();
            CompletedOrder.AssignedChef->AddToXP(CompletedOrder.XP + (CompletedOrder.AssignedChef->ChefLevel * 5));
        }

        // 손님 음식 수령 및 만족도, 인벤토리 아이템 소모 등 핵심 정산 프로세스 실행
        CompleteOrderProcess(CompletedOrder);
       
        // 대기열에서 최종 제거 및 알림
        KitchenOrderQueue.RemoveAt(i);
        OnKitchenOrderRemoved.Broadcast(CompletedOrder);
    }

    // 3. 완료 처리 후 남은 대기열이 있다면 다음 요리사 재배정
    AssignChef();

    // 4. 모든 대기열이 비었다면 타이머를 꺼서 자원 낭비 방지
    if (KitchenOrderQueue.IsEmpty())
    {
        GetWorld()->GetTimerManager().ClearTimer(OrderTimerHandle);
    }
}

void UKitchenOrderSubsystem::CompleteOrderProcess(const FKitchenOrder& Order)
{
    AInnCustomer* Customer = Cast<AInnCustomer>(Order.OrderedCustomer);
    if (!IsValid(Customer)) return;

    // 손님 음식 수령 애니메이션/상태 트리거
    Customer->ReceiveFood();

    UInnManagerSubsystem* InnSystem = GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
    if (!InnSystem) return;

    int32 FinalSatisfaction = Order.DefaultSatisfaction;

    // 특별 재료가 인벤토리에 있을 시 소모 및 만족도 2배 보너스 적용
    if (DonPlayerState && DonPlayerState->GetInventoryComponent())
    {
        FItem Ingredient = UDonItemLibrary::FindItemByName(this, Order.SpecialIngredient);
        int32 InvIndex = DonPlayerState->GetInventoryComponent()->FindItemInInventory(Ingredient);
        
        if (InvIndex != INDEX_NONE)
        {
            FinalSatisfaction *= 2;
            DonPlayerState->GetInventoryComponent()->RemoveItemByIndex(InvIndex);
        }
    }
    
    // 고객이 속한 그룹의 전반적인 만족도 누적
    UInnCustomerGroup* Group = InnSystem->GetGroupInfo(Customer->GetGroupID());
    if (Group)
    {
        Group->AddToSatisfaction(FinalSatisfaction);
    }

    // 영수증/매출 통계용 완료된 주문 데이터 추가
    FCompletedFoodOrder HistoryLog;
    HistoryLog.ChefLevel			= IsValid(Order.AssignedChef) ? Order.AssignedChef->GetChefLevel() : 1;
    HistoryLog.FoodName				= Order.CuisineName;
    HistoryLog.FoodPrice			= Order.Price;
    HistoryLog.CustomerID			= Customer->GetID();
    HistoryLog.OrderID				= Order.OrderID;
    HistoryLog.CustomerSatisfaction = FinalSatisfaction;

    CompletedFoodOrders.Add(HistoryLog);
}


// ============================================================================
// 데이터 조회 및 주방 마감
// ============================================================================

FCompletedFoodOrder* UKitchenOrderSubsystem::GetCompletedOrder(FGuid ID)
{
    for (FCompletedFoodOrder& Order : CompletedFoodOrders)
    {
        if (Order.CustomerID == ID)
        {
            return &Order;
        }
    }
    return nullptr;
}

void UKitchenOrderSubsystem::CloseKitchen()
{
    KitchenOrderQueue.Empty();
    GetWorld()->GetTimerManager().ClearTimer(OrderTimerHandle);
}