// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Attack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "DonGameplayTags.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

void UBTTask_Attack::RotateToTarget(UBehaviorTreeComponent& OwnerComp, AActor* TargetActor)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	FVector Direction = (TargetActor->GetActorLocation() - ControlledPawn->GetActorLocation()).GetSafeNormal();
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	ControlledPawn->SetActorRotation(LookAtRotation);
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController ? AIController->GetPawn() : nullptr;
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn);

	if (!ASC) return EBTNodeResult::Failed;

	// 1. 어빌리티 종료를 감지할 람다 또는 함수 바인딩
	// 태스크가 끝나기 전까지 유효해야 하므로 OwnerComp를 캡처합니다.
	TSharedPtr<FDelegateHandle> Handle = MakeShared<FDelegateHandle>();
	*Handle = ASC->OnAbilityEnded.AddLambda([this, &OwnerComp, ASC, Handle](const FAbilityEndedData& AbilityData)
	{
		// 종료된 어빌리티가 우리가 실행한 태그를 가지고 있는지 확인
		if (AbilityData.AbilitySpecHandle.IsValid() && AbilityData.AbilityThatEnded->AbilityTags.HasTag(AttackTag))
		{
			// 델리게이트 해제 (메모리 누수 방지)
			ASC->OnAbilityEnded.Remove(*Handle);
            
			// 2. 태스크 완료 알림 (성공 리턴)
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	});

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (bRotateToTarget) RotateToTarget(OwnerComp, PlayerPawn);
	
	// 3. 어빌리티 실행
	bool bSuccess = ASC->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());

	if (!bSuccess)
	{
		ASC->OnAbilityEnded.Remove(*Handle);
		return EBTNodeResult::Failed;
	}

	// GA가 끝날 때까지 기다려야 하므로 InProgress 리턴
	return EBTNodeResult::InProgress;
}