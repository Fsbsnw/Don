// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CuisineAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KitchenOrderSubsystem.generated.h"

class ADonPlayerState;
class AInnSeat;
class AInnCustomer;
class AInnChef;

USTRUCT(BlueprintType)
struct FCompletedFoodOrder
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ChefLevel = 1;

	UPROPERTY(BlueprintReadWrite)
	FName FoodName;
	
	UPROPERTY(BlueprintReadWrite)
	int32 FoodPrice = 0;

	UPROPERTY()
	FGuid OrderID = FGuid();

	UPROPERTY()
	FGuid CustomerID = FGuid();

	UPROPERTY()
	int32 CustomerSatisfaction = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCharged = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKitchenOrderChanged, FKitchenOrder, KitchenOrder);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKitchenOrderUpdated, const TArray<FKitchenOrder>&, KitchenOrders);

/**
 * 
 */
UCLASS()
class DON_API UKitchenOrderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
// ============================================================================
// 델리게이트 및 이벤트
// ============================================================================
    
    UPROPERTY(BlueprintAssignable, Category = "Kitchen|Events")
    FOnKitchenOrderChanged OnKitchenOrderAdded;

    UPROPERTY(BlueprintAssignable, Category = "Kitchen|Events")
    FOnKitchenOrderChanged OnKitchenOrderRemoved;

    UPROPERTY(BlueprintAssignable, Category = "Kitchen|Events")
    FOnKitchenOrderUpdated OnKitchenOrderUpdated;

// ============================================================================
// 주문 대기열 및 조회 함수
// ============================================================================
    
    /** 현재 주방에 등록된 모든 주문 대기열을 참조로 반환합니다. */
    UFUNCTION(BlueprintCallable, Category = "Kitchen|Order")
    TArray<FKitchenOrder>& GetKitchenOrderQueue() { return KitchenOrderQueue; }

    /** 특정 손님 ID를 기반으로 이미 완료된 정산 영수증 데이터를 찾아 반환합니다. */
    FCompletedFoodOrder* GetCompletedOrder(FGuid ID);

    /** 조리가 시작되지 않고 대기열에서 가장 오래 머문 주문의 ID를 반환합니다. */
    FGuid FindNextQueuedOrderID() const;

// ============================================================================
// 주방 영업 함수
// ============================================================================
    
    /** 새로운 요리 주문을 대기열에 등록하고 가용한 요리사가 있다면 즉시 조리를 시작합니다. */
    FKitchenOrder EnqueueKitchenOrder(FKitchenOrder& Order);

    /** 대기 중인 주문과 놀고 있는 요리사를 매칭하여 조리를 시작하도록 지시합니다. */
    void AssignChef();

    /** 타이머에 의해 주기적으로 호출되며, 조리 중인 주문들의 남은 시간을 차감하고 완료 처리를 수행합니다. */
    void UpdateKitchenOrders();

    /** 주방 영업을 종료하고 타이머 및 대기열을 강제로 비웁니다. */
    void CloseKitchen();

// ============================================================================
// 요리사 등록 및 관리 함수
// ============================================================================
    
    /** 월드에 스폰된 요리사 액터를 서브시스템의 관리 명부에 등록합니다. */
    void RegisterChef(AInnChef* Chef) { Chefs.AddUnique(Chef); }

    /** 요리사 액터를 관리 명부에서 제외합니다. */
    void UnregisterChef(AInnChef* Chef) { if (Chefs.Contains(Chef)) Chefs.Remove(Chef); }

    /** 현재 고용된 상태이면서 요리를 하고 있지 않은 요리사 한 명을 찾아 반환합니다. */
    AInnChef* FindIdleChef();

public:
// ============================================================================
// 히스토리 저장 데이터
// ============================================================================
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kitchen|History")
    TArray<FCompletedFoodOrder> CompletedFoodOrders;

protected:
// ============================================================================
// 서브시스템 생명 주기 및 공통 초기화
// ============================================================================
    
    /** 월드 진입 또는 게임 시작 시 플레이어 상태를 캐싱하고 요리사 고용 상태를 초기화합니다. */
    UFUNCTION(BlueprintCallable, Category = "Kitchen|System")
    void BroadcastInitialValues();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kitchen|System")
    ADonPlayerState* DonPlayerState;

private:
// ============================================================================
// 내부 정산 프로세스 및 컨테이너
// ============================================================================
    
    /** 완료된 주문의 만족도 계산, 아이템 차감, 히스토리 저장을 내부적으로 처리합니다. */
    void CompleteOrderProcess(const FKitchenOrder& Order);

    UPROPERTY()
    TArray<FKitchenOrder> KitchenOrderQueue;

    UPROPERTY()
    TArray<AInnChef*> Chefs;
    
    FTimerHandle OrderTimerHandle;

    /** 주문 상태를 갱신할 타이머 주기 (기본값 0.1초) */
    float TickTimer = 0.1f;
};
