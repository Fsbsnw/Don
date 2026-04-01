// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_CheckPlayerInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckPlayerInRange::UBTService_CheckPlayerInRange()
{
	NodeName = TEXT("Find Player");
}

void UBTService_CheckPlayerInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APawn* AIPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (AIPawn == nullptr) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	float Dist = FVector::Dist(AIPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
	if (Dist < TargetRadius)
	{
		// 시야 체크
		FHitResult HitResult;
		FVector Start = AIPawn->GetActorLocation() + FVector(0,0,50);
		FVector End = PlayerPawn->GetActorLocation() + FVector(0,0,50);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(AIPawn);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Pawn,
			Params
		);

		if (bHit && HitResult.GetActor() == PlayerPawn)
		{
			if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
			{
				BlackboardComponent->SetValueAsObject(TEXT("TargetPlayer"), PlayerPawn);
				BlackboardComponent->SetValueAsFloat(TEXT("DistanceToTarget"), Dist);
			}
		}
	}
	else
	{
		if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
		{
			BlackboardComponent->ClearValue(TEXT("TargetPlayer"));
			BlackboardComponent->ClearValue(TEXT("DistanceToTarget"));
		}
	}
}
