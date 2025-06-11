// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_FindNearestEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_FindNearestEnemy::UBTService_FindNearestEnemy()
{
	NodeName = TEXT("Find Nearest Enemy");
}

void UBTService_FindNearestEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (AIPawn == nullptr) return;
	
	TArray<AActor*> FoundActors;

	if (OwnerComp.GetOwner()->ActorHasTag(FName("Player")))
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), FoundActors);
	}
	else
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundActors);
	}

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

	if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		if (NearestEnemy)
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetEnemy"), NearestEnemy);
			BlackboardComponent->SetValueAsFloat(TEXT("DistanceToFollow"), NearestDistance);
		}
		else
		{
			BlackboardComponent->ClearValue(TEXT("TargetEnemy"));
			BlackboardComponent->ClearValue(TEXT("DistanceToFollow"));
		}
	}
}
