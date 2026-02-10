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

bool UKitchenOrderSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (const UWorld* World = Outer->GetWorld())
	{
		return World->GetMapName().Contains("InnMap");
	}
	return false;
}

void UKitchenOrderSubsystem::BroadcastInitialValues()
{
	DonPlayerState = CastChecked<ADonPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	
	OnKitchenOrderUpdated.Broadcast(KitchenOrderQueue);
}

void UKitchenOrderSubsystem::AssignChef()
{
	for (FKitchenOrder& Order : KitchenOrderQueue)
	{
		if (Order.bIsCooking || Order.RemainingTime <= 0.f) continue;
		
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
		else break;
	}
}

FKitchenOrder UKitchenOrderSubsystem::EnqueueKitchenOrder(FKitchenOrder& Order)
{
	Order.OrderID = FGuid::NewGuid();
	Order.RemainingTime = Order.CookingTime;
	
	KitchenOrderQueue.Add(Order);
	AssignChef();
	
	OnKitchenOrderAdded.Broadcast(Order);
	return Order;
}

FGuid UKitchenOrderSubsystem::FindNextQueuedOrderID() const
{
	for (const FKitchenOrder& Order : KitchenOrderQueue)
	{
		if (!Order.bIsCooking)
		{
			return Order.OrderID;
		}
	}
	return FGuid();
}

AInnChef* UKitchenOrderSubsystem::FindIdleChef()
{
	for (AInnChef* Chef : Chefs)
	{
		if (!Chef->IsCooking()) return Chef;
	}
	return nullptr;
}

void UKitchenOrderSubsystem::UpdateKitchenOrders()
{
	TArray<int32> IndexToRemove;
	for (int32 i = 0; i < KitchenOrderQueue.Num(); i++)
	{
		FKitchenOrder& Order = KitchenOrderQueue[i];
		if (Order.bIsCooking && Order.RemainingTime > 0)
		{
			Order.RemainingTime -= TickTimer;

			if (Order.RemainingTime <= 0.f)
			{
				Order.bIsCooking = false;
				IndexToRemove.Add(i);
			}
		}
	}

	OnKitchenOrderUpdated.Broadcast(KitchenOrderQueue);

	// 내림차순 제거
	IndexToRemove.Sort(TGreater<int32>());

	for (int32 i : IndexToRemove)
	{
		FKitchenOrder Order = KitchenOrderQueue[i];
		Order.AssignedChef->EndOrder();

		UInnManagerSubsystem* InnSystem = GetGameInstance()->GetSubsystem<UInnManagerSubsystem>();
		
		if (AInnCustomer* Customer = Cast<AInnCustomer>(Order.OrderedCustomer))
		{
			Customer->ReceiveFood();

			FItem Ingredient = UDonItemLibrary::FindItemByName(this, Order.SpecialIngredient);
			int32 Index = DonPlayerState->GetInventoryComponent()->FindItemInInventory(Ingredient);
			int32 FinalSatisfaction = Order.DefaultSatisfaction;
			
			if (Index != INDEX_NONE)
			{
				FinalSatisfaction *= Customer->GetLevel() + 2;
				DonPlayerState->GetInventoryComponent()->RemoveItem(Index);
			}
			
			UInnCustomerGroup* Group = InnSystem->GetGroupInfo(Customer->GetGroupID());
			Group->AddToSatisfaction(FinalSatisfaction);

			// Save Completed Order
			FCompletedFoodOrder CompletedOrder;
			CompletedOrder.ChefLevel = Order.AssignedChef->GetChefLevel();
			CompletedOrder.FoodName = Order.CuisineName;
			CompletedOrder.FoodPrice = Order.Price;
			CompletedOrder.CustomerID = Customer->GetID();
			CompletedOrder.OrderID = Order.OrderID;

			CompletedFoodOrders.Add(CompletedOrder);
		}
		
		KitchenOrderQueue.RemoveAt(i);
		OnKitchenOrderRemoved.Broadcast(Order);
	}

	AssignChef();

	// 타이머 종료
	if (KitchenOrderQueue.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(OrderTimerHandle);
	}
}

FCompletedFoodOrder UKitchenOrderSubsystem::GetCompletedOrder(FGuid ID)
{
	FCompletedFoodOrder FoundOrder;
	
	for (FCompletedFoodOrder& Order : CompletedFoodOrders)
	{
		if (Order.CustomerID == ID)
		{
			FoundOrder = Order;
			return FoundOrder;
		}
	}
	return FoundOrder;
}