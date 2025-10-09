// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DonEnemyPawn.h"

#include "BrainComponent.h"
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
#include "UI/Widget/HealthBarWidget.h"

ADonEnemyPawn::ADonEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AttributeSet = CreateDefaultSubobject<UDonAttributeSet>("AttributeSet");
	AbilitySystemComponent = CreateDefaultSubobject<UDonAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule Component");
	Capsule->SetCollisionObjectType(ECC_GameTraceChannel2);
	Capsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	SetRootComponent(Capsule);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh");
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetupAttachment(Capsule);
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	MovementComponent->SetUpdatedComponent(RootComponent);
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("Health Bar Widget Component");
}

UAbilitySystemComponent* ADonEnemyPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADonEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
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

	AddCharacterAbilities();
	
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

void ADonEnemyPawn::InitializeDefaultAttributes()
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, GetCharacterLevel_Implementation());
	SecondaryEffectHandle = ApplyEffectToSelf(DefaultSecondaryAttributes, GetCharacterLevel_Implementation());
	MaxVitalEffectHandle = ApplyEffectToSelf(DefaultMaxVitalAttributes, GetCharacterLevel_Implementation());
	ApplyEffectToSelf(DefaultVitalAttributes, GetCharacterLevel_Implementation());
}

void ADonEnemyPawn::AddCharacterAbilities()
{
	UDonAbilitySystemComponent* ASC = CastChecked<UDonAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	ASC->AddCharacterAbilities(StartupAbilities);
	ASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
	ASC->AddCharacterStartupAbilities(StartupCommonAbilities);
}

void ADonEnemyPawn::ResetMaterials()
{
}

FActiveGameplayEffectHandle ADonEnemyPawn::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	return GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
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
