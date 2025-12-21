// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DonAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Inn/Character/InnCustomer.h"

ADonAIController::ADonAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
}