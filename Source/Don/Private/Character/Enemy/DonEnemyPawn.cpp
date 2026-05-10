// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DonEnemyPawn.h"

#include "BrainComponent.h"
#include "DonGameModeBase.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/DonAbilityLibrary.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"
#include "AI/DonAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameInstance/SubSystem/EnemyManagerSubsystem.h"
#include "Inventory/DonItemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/HealthBarWidget.h"

ADonEnemyPawn::ADonEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AttributeSet = CreateDefaultSubobject<UDonAttributeSet>("AttributeSet");
	AbilitySystemComponent = CreateDefaultSubobject<UDonAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	MovementComponent->SetUpdatedComponent(RootComponent);
	
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("Health Bar Widget Component");
	HealthBarComponent->SetupAttachment(CapsuleComponent);
}


void ADonEnemyPawn::Destroyed()
{
	if (ADonGameModeBase* GameModeBase = Cast<ADonGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameModeBase->AddToSpawnedEnemies(-1);
	}
	GetWorld()->GetTimerManager().ClearTimer(HealthVisibilityTimerHandle);
	
	Super::Destroyed();
}

void ADonEnemyPawn::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UDonAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
	UDonAttributeSet* DonAttributeSet = CastChecked<UDonAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		DonAttributeSet->GetHealthAttribute()).AddLambda(
			[this, DonAttributeSet](const FOnAttributeChangeData& Data)
			{
				SetHealthVisibility(true);
				const float NewValue = DonAttributeSet->GetHealth() / DonAttributeSet->GetMaxHealth();
				SetHealthText(DonAttributeSet->GetHealth(), DonAttributeSet->GetMaxHealth());
				SetHealthPercent(NewValue);

				if (GetWorld()->GetTimerManager().IsTimerActive(HealthVisibilityTimerHandle))
				{
					GetWorld()->GetTimerManager().ClearTimer(HealthVisibilityTimerHandle);
				}

				FTimerDelegate HealthVisibilityDelegate;
				HealthVisibilityDelegate.BindLambda(
					[this]()
					{
						SetHealthVisibility(false);
					}
				);

				GetWorld()->GetTimerManager().SetTimer(
					HealthVisibilityTimerHandle,
					HealthVisibilityDelegate,
					3.f,
					false
				);
			}
	);
}

void ADonEnemyPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeDefaultAttributes();
	AddCharacterAbilities();
	InitAbilityActorInfo();

	SetHealthVisibility(false);
	
	// 적군 액터 관리 매니저 등록
	if (ADonAIController* DonAIController = Cast<ADonAIController>(NewController))
	{
		DonAIController->RunBehaviorTree(BehaviorTree);
		if (!TestBTCondition) DonAIController->GetBrainComponent()->StopLogic("Init");
		if (UEnemyManagerSubsystem* Manager = GetGameInstance()->GetSubsystem<UEnemyManagerSubsystem>())
		{
			Manager->RegisterEnemyPawn(this);
		}
	}
}

void ADonEnemyPawn::Die_Implementation(const FVector& DeathImpulse, float ItemDropRate)
{
	Super::Die_Implementation(DeathImpulse, ItemDropRate);

	if (DeathSound) UGameplayStatics::PlaySound2D(this, DeathSound, 1);

	const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = GetActorRotation();
	FEnemyClassInfo EnemyClassInfo = UDonAbilityLibrary::FindEnemyClassInfo(this, EnemyClass);

	UDonItemLibrary::SpawnLootableXP(this, EnemyClassInfo.DroppableXP, SpawnLocation, SpawnRotation);
	UDonItemLibrary::SpawnLootableMoney(this, EnemyClassInfo.DroppableMoney, FMath::RandRange(0, 3), SpawnLocation, SpawnRotation);
	for (FLootableItem& LootableItem : LootableItems)
	{
		// Normalized Rate
		float AdjustedRate = ItemDropRate * 0.01f * LootableItem.DropRate;
		UDonItemLibrary::SpawnLootableItem(this, LootableItems, SpawnLocation, SpawnRotation, AdjustedRate);
	}

	if (DeathEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathEffect, SpawnLocation);
	
	Destroy();
}

void ADonEnemyPawn::SetHealthVisibility(bool NewState)
{
	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(NewState);
	}
}

void ADonEnemyPawn::SetHealthText(float NewHealth, float NewMaxHealth)
{
	if (HealthBarComponent)
	{
		UUserWidget* Widget = HealthBarComponent->GetWidget();
		if (Widget)
		{
			UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(Widget);
			if (HealthBarWidget)
			{
				HealthBarWidget->Text_HP->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::RoundToInt(NewHealth), FMath::RoundToInt(NewMaxHealth))));
			}
		}
	}
}

void ADonEnemyPawn::SetHealthPercent(float NewHealth)
{
	if (HealthBarComponent)
	{
		UUserWidget* Widget = HealthBarComponent->GetWidget();
		if (Widget)
		{
			UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(Widget);
			if (HealthBarWidget)
			{
				HealthBarWidget->ProgressBar_HP->SetPercent(NewHealth);
			}
		}
	}
}
void ADonEnemyPawn::SetKnockbackState_Implementation(bool NewState, const FVector& Force)
{
	if (CapsuleComponent == nullptr || SkeletalMesh == nullptr) return;
	
	if (NewState)
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		SkeletalMesh->GetAnimInstance()->Montage_Stop(0.f);

		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkeletalMesh->SetSimulatePhysics(true);
		SkeletalMesh->SetPhysicsBlendWeight(1.0f);
		SkeletalMesh->bBlendPhysics = false;
		SkeletalMesh->WakeAllRigidBodies();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *SkeletalMesh->GetName());
		FVector ScaledForce = FVector(Force.X * (ForceMultiplier / TestXDivide), Force.Y * ForceMultiplier, Force.Z * ForceMultiplier); 
		SkeletalMesh->AddImpulse(ScaledForce, NAME_None, true);

		FTimerDelegate KnockbackCollisionTimerDelegate;
		KnockbackCollisionTimerDelegate.BindLambda(
			[this]()
			{
				if (!IsValid(this) || !IsValid(this->SkeletalMesh)) return;
				if (!bKnockback)
				{
					GetWorld()->GetTimerManager().ClearTimer(KnockbackCollisionTimerHandle);
					return;
				}
				const FVector PelvisLocation = this->SkeletalMesh->GetSocketLocation(BodyCenterBone);
				this->CapsuleComponent->SetWorldLocation(PelvisLocation, true);
			}
		);
		GetWorld()->GetTimerManager().SetTimer(KnockbackCollisionTimerHandle, KnockbackCollisionTimerDelegate, 0.1f, true);
		
		bKnockback = true;
	}
	else
	{
		// 1️⃣ 현재 캐릭터의 중요 위치(골반, 목) 가져오기
		const FVector NeckLocation = SkeletalMesh->GetSocketLocation(NeckBone);
		const FVector PelvisLocation = SkeletalMesh->GetSocketLocation(BodyCenterBone);

		// 2️⃣ 물리 및 충돌 설정 변경 (일시적으로 비활성화)
		SkeletalMesh->SetSimulatePhysics(false);
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		// 3️⃣ 캡슐의 새 회전값 계산 (Pelvis → Neck 방향을 기준으로 회전)
		FRotator NewCapsuleRotation = (PelvisLocation - NeckLocation).Rotation();
		NewCapsuleRotation.Pitch = 0.f;
		NewCapsuleRotation.Roll = 0.f;

		// 4️⃣ 메시의 오른쪽 벡터를 사용해 캡슐 회전 보정
		FRotator MeshRotation = SkeletalMesh->GetSocketRotation(BodyCenterBone);
		FVector SocketUpVector = FRotationMatrix(MeshRotation).GetUnitAxis(EAxis::Y);
		float Dot = FVector::DotProduct(SocketUpVector, FVector::UpVector);

		if (Dot < 0.f)
		{
			NewCapsuleRotation.Yaw += 180.f;
			bForwardRagdoll = false;
		}
		else
		{
			bForwardRagdoll = true;
		}

		// 5️⃣ 캡슐을 새로운 위치 및 회전값으로 설정
		CapsuleComponent->SetWorldLocationAndRotation(PelvisLocation, NewCapsuleRotation, true);
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		// 6️⃣ 메시를 캡슐에 부착하고 위치 및 회전값 조정
		SkeletalMesh->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SkeletalMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
		SkeletalMesh->ResetAllBodiesSimulatePhysics();
		SkeletalMesh->RecreatePhysicsState();

		// 7️⃣ 애니메이션 실행
		SetKnockback(false);
	}
}

void ADonEnemyPawn::CheckAndUpdateAILOD(const FVector& PlayerLocation)
{
	if (TestBTCondition) return;
	float Distance = FVector::Dist(GetActorLocation(), PlayerLocation);
	FVector Direction = PlayerLocation - GetActorLocation();
	Direction.Normalize();
	CurrentMoveDirection = Direction;
	
	// 1. 이번에 되어야 할 '목표' LOD 레벨을 계산합니다.
	int32 DesiredAILODLevel = (Distance > AILODThreshold) ? 0 : 1;

	// 2. ✅ 가장 중요한 부분: 목표 레벨이 현재 레벨과 같다면, 아무것도 하지 않고 함수를 종료합니다.
	if (DesiredAILODLevel == CurrentAILODLevel)
	{
		// 단, LOD 0 상태가 유지될 때는 MoveToLocation을 계속 갱신해 줄 수 있습니다.
		if (CurrentAILODLevel == 0 && Controller)
		{
			const float DistToLastTarget = FVector::Dist(PlayerLocation, CurrentTargetLocation);
			if (DistToLastTarget < ResetDistanceThreshold) return;
			
			if (AAIController* AIController = Cast<AAIController>(Controller))
			{
				AIController->MoveToLocation(PlayerLocation);
				CurrentTargetLocation = PlayerLocation;
			}
		}
		return;
	}

	// 3. LOD 레벨이 바뀌었으므로, 현재 상태를 새로운 레벨로 업데이트합니다.
	CurrentAILODLevel = DesiredAILODLevel;
    
	if (AAIController* AIController = Cast<AAIController>(Controller))
	{
		if (CurrentAILODLevel == 0)
		{
			// [LOD 0 으로 변경]
			AIController->GetBrainComponent()->StopLogic("Far LOD Level");
			AIController->MoveToLocation(PlayerLocation);
			CurrentTargetLocation = PlayerLocation;
		}
		else // CurrentAILODLevel == 1
		{
			// [LOD 1 으로 변경]
			// 이 블록은 이제 상태가 0 -> 1로 바뀔 때 '단 한 번만' 실행됩니다.
			AIController->StopMovement();
			CurrentMoveDirection = FVector::ZeroVector;
			
			if (AIController->GetBlackboardComponent())
			{
				// 위치 초기화
				AIController->GetBlackboardComponent()->ClearValue(TEXT("TargetLocation")); 
			}
			
			AIController->GetBrainComponent()->RestartLogic();
		}
	}
}
