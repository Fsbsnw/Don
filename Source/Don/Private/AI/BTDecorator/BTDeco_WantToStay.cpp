// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDeco_WantToStay.h"

#include "AIController.h"
#include "GameInstance/SubSystem/InnManagerSubsystem.h"
#include "Inn/Character/InnCustomer.h"
#include "Kismet/GameplayStatics.h"

UBTDeco_WantToStay::UBTDeco_WantToStay()
{
	NodeName = "Want To Stay";
}

bool UBTDeco_WantToStay::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	
	if (AInnCustomer* Customer = Cast<AInnCustomer>(Pawn))
	{
		UInnManagerSubsystem* InnSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInnManagerSubsystem>();
		ECustomerInnState InnState = InnSystem->GetGroupInnState(Customer->GetGroupID());
		
		return InnState == ECustomerInnState::Room;
	}
	return false;
}