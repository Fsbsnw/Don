// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDeco_WantToStay.h"

#include "AIController.h"
#include "Inn/Character/InnCustomer.h"
#include "Kismet/GameplayStatics.h"

UBTDeco_WantToStay::UBTDeco_WantToStay()
{
	NodeName = "Want To Stay";
}

bool UBTDeco_WantToStay::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	// UKitchenOrderSubsystem* KitchenSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UKitchenOrderSubsystem>();
	// if (KitchenSystem == nullptr) return false;
	
	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		Customer->bGoToRoom = FMath::RandBool();
		// URoomSystem으로 빈방 확인
		return Customer->bGoToRoom;
	}
	
	return false;
}
