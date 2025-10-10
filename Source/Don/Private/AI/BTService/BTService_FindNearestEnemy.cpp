// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_FindNearestEnemy.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_FindNearestEnemy::UBTService_FindNearestEnemy()
{
	NodeName = TEXT("Find Player");
}

void UBTService_FindNearestEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APawn* AIPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (AIPawn == nullptr) return;
	
	TArray<AActor*> FoundActors;

	// 플레이어 찾기
	if (AIPawn->ActorHasTag(FName("Enemy")))
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundActors);
	}


	// 가장 가까운 플레이어 찾기
	AActor* NearestEnemy = nullptr;
	const FVector SourceLocation = AIPawn->GetActorLocation();
	
	float NearestDistance = 10000.f;
	for (AActor* FoundActor : FoundActors)
	{
		const FVector TargetLocation = FoundActor->GetActorLocation();
		float CurrentDistance = FVector::Dist(SourceLocation, TargetLocation);
		if (CurrentDistance < NearestDistance)
		{
			NearestDistance = CurrentDistance;
			NearestEnemy = FoundActor;
		}
	}

	// 블랙보드 값 구하기
	if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		if (NearestEnemy)
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetPlayer"), NearestEnemy);

			const FVector Center = NearestEnemy->GetActorLocation();
			FVector TargetDestination = Center;

			if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation RandomPoint;
				
				// Target 주변 랜덤 좌표 찾기
				if (NavSys->GetRandomPointInNavigableRadius(Center, RandomRadius, RandomPoint))
				{
					TargetDestination = RandomPoint.Location;
				}
			}

			// 랜덤 좌표 저장
			BlackboardComponent->SetValueAsVector(TEXT("TargetLocation"), TargetDestination);
		}
		else
		{
			BlackboardComponent->ClearValue(TEXT("TargetPlayer"));
			BlackboardComponent->ClearValue(TEXT("TargetLocation"));
		}
	}
}
