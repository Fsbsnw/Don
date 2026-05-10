// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SubSystem/EnemyManagerSubsystem.h"

#include "Character/Enemy/DonEnemyPawn.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

bool UEnemyManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (const UWorld* World = Outer->GetWorld())
	{
		return World->GetMapName().Contains("Test_Default");
	}
	return false;
}

void UEnemyManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GetWorld()->GetTimerManager().SetTimer(
		UpdateAIsTimerHandle,      // 타이머 핸들z
		this,                      // 함수를 호출할 오브젝트
		&UEnemyManagerSubsystem::UpdateAILOD, // 호출할 함수
		0.1f,                      // 호출 간격 (초)
		true                       // 반복 여부
	);

	UE_LOG(LogTemp, Warning, TEXT("AI Subsystem Initialized and Timer Started!"));
}

void UEnemyManagerSubsystem::UpdateAILOD()
{
	// 순회 시작 시, Max 인덱스 재설정
	if (NextEnemyToUpdateIndex == 0) MaxEnemyCount = EnemyPawns.Num();
	
	// 플레이어 위치 가져오기
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!Player) return;
	const FVector PlayerLocation = Player->GetActorLocation();
	const int32 MaxRange = FMath::Min(MaxEnemyCount, NextEnemyToUpdateIndex + MaxEnemiesPerFrame);
	
	for (int32 i = NextEnemyToUpdateIndex; i < MaxRange; ++i)
	{
		if (IsValid(EnemyPawns[NextEnemyToUpdateIndex]))
		{
			EnemyPawns[NextEnemyToUpdateIndex]->CheckAndUpdateAILOD(PlayerLocation);
			if (EnemyPawns[NextEnemyToUpdateIndex]->CurrentAILODLevel == 0) ++TestCountNotBT;
			else ++TestCountBT;
		}
		++NextEnemyToUpdateIndex;
	}

	// 처리 완료 후, 다음 인덱스 초기화 및 Destroy된 객체들 제거 
	if (NextEnemyToUpdateIndex >= MaxEnemyCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("BT : %d, Not BT : %d"), TestCountBT, TestCountNotBT);
		TestCountBT = 0, TestCountNotBT = 0;
		NextEnemyToUpdateIndex = 0;
		EnemyPawns.Remove(nullptr);
	}
}

void UEnemyManagerSubsystem::RegisterEnemyPawn(ADonEnemyPawn* EnemyPawn)
{
	EnemyPawns.AddUnique(EnemyPawn);
}
