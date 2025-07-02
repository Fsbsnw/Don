// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DonEnemy.h"

#include "DonGameModeBase.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"
#include "AI/DonAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Inventory/DonItemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/HealthBarWidget.h"

ADonEnemy::ADonEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UDonAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UDonAttributeSet>("AttributeSet");

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("Health Bar Widget Component");
}

void ADonEnemy::Destroyed()
{
	if (ADonGameModeBase* GameModeBase = Cast<ADonGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameModeBase->AddToSpawnedEnemies(-1);
	}
	GetWorld()->GetTimerManager().ClearTimer(HealthVisibilityTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(KnockbackCollisionTimerHandle);
	
	Super::Destroyed();
}

void ADonEnemy::SetKnockbackState_Implementation(bool NewState, const FVector& Force)
{
	Super::SetKnockbackState_Implementation(NewState, Force);
	
	SetKnockbackState(NewState, Force);
}

void ADonEnemy::SetMeshInitState()
{

}

void ADonEnemy::SetKnockbackState(bool NewState, FVector Force)
{
	if (GetCapsuleComponent() == nullptr || GetMesh() == nullptr || bBossEnemy) return;
	
	if (NewState)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		FVector ScaledForce = FVector(Force.X * (ForceMultiplier / TestXDivide), Force.Y * ForceMultiplier, Force.Z * ForceMultiplier); 
		GetMesh()->AddImpulse(ScaledForce, NAME_None, true);

		FTimerDelegate KnockbackCollisionTimerDelegate;
		KnockbackCollisionTimerDelegate.BindLambda(
			[this]()
			{
				if (!bKnockback)
				{
					GetWorld()->GetTimerManager().ClearTimer(KnockbackCollisionTimerHandle);
					return;
				}
				const FVector PelvisLocation = GetMesh()->GetSocketLocation(FName("pelvis"));
				GetCapsuleComponent()->SetWorldLocation(PelvisLocation);
			}
		);
		GetWorld()->GetTimerManager().SetTimer(KnockbackCollisionTimerHandle, KnockbackCollisionTimerDelegate, 0.1f, true);
		
		SetKnockback(true);
	}
	else
	{
		// 1️⃣ 현재 캐릭터의 중요 위치(골반, 목) 가져오기
		const FVector NeckLocation = GetMesh()->GetSocketLocation(FName("neck_01"));
		const FVector PelvisLocation = GetMesh()->GetSocketLocation(FName("pelvis"));

		// 2️⃣ 물리 및 충돌 설정 변경 (일시적으로 비활성화)
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		// 3️⃣ 캡슐의 새 회전값 계산 (Pelvis → Neck 방향을 기준으로 회전)
		FRotator NewCapsuleRotation = (PelvisLocation - NeckLocation).Rotation();
		NewCapsuleRotation.Pitch = 0.f;
		NewCapsuleRotation.Roll = 0.f;

		// 4️⃣ 메시의 오른쪽 벡터를 사용해 캡슐 회전 보정
		FRotator MeshRotation = GetMesh()->GetSocketRotation(FName("pelvis"));
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
		GetCapsuleComponent()->SetWorldLocationAndRotation(PelvisLocation, NewCapsuleRotation);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		// 6️⃣ 메시를 캡슐에 부착하고 위치 및 회전값 조정
		GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));

		// 7️⃣ 애니메이션 실행
		SetKnockback(false);
	}
}

void ADonEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AddCharacterAbilities();
	DonAIController = Cast<ADonAIController>(NewController);

	DonAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	DonAIController->RunBehaviorTree(BehaviorTree);
}

void ADonEnemy::Die_Implementation(const FVector& DeathImpulse, float ItemDropRate)
{
	Super::Die_Implementation(DeathImpulse, ItemDropRate);

	const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = GetActorRotation();
	FCharacterClassInfo CharacterClassInfo = UDonItemLibrary::FindCharacterClassInfo(this, CharacterClass);

	UDonItemLibrary::SpawnLootableXP(this, CharacterClassInfo.DroppableXP, SpawnLocation, SpawnRotation);
	UDonItemLibrary::SpawnLootableMoney(this, CharacterClassInfo.DroppableMoney, FMath::RandRange(0, 3), SpawnLocation, SpawnRotation);
	for (FLootableItem& LootableItem : LootableItems)
	{
		// Normalized Rate
		float AdjustedRate = ItemDropRate * 0.01f * LootableItem.DropRate;
		UDonItemLibrary::SpawnLootableItem(this, LootableItems, SpawnLocation, SpawnRotation, AdjustedRate);
	}

	if (DeathEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathEffect, SpawnLocation);
	Destroy();
}

void ADonEnemy::SetHealthPercent(float NewValue)
{
	if (HealthBarComponent)
	{
		UUserWidget* Widget = HealthBarComponent->GetWidget();
		if (Widget)
		{
			UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(Widget);
			if (HealthBarWidget)
			{
				HealthBarWidget->ProgressBar_HP->SetPercent(NewValue);
			}
		}
	}
}

void ADonEnemy::SetHealthText(float NewValue, float NewMaxValue)
{
	if (HealthBarComponent)
	{
		UUserWidget* Widget = HealthBarComponent->GetWidget();
		if (Widget)
		{
			UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(Widget);
			if (HealthBarWidget)
			{
				HealthBarWidget->Text_HP->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::RoundToInt(NewValue), FMath::RoundToInt(NewMaxValue))));
			}
		}
	}
}

void ADonEnemy::SetHealthVisibility(bool State)
{
	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(State);
	}
}

void ADonEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
	HealthBarComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

void ADonEnemy::InitAbilityActorInfo()
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
