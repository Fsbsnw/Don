// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/DonEnemyCharacter.h"

#include "DonGameModeBase.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/DonAbilityLibrary.h"
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

ADonEnemyCharacter::ADonEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);

	AbilitySystemComponent = CreateDefaultSubobject<UDonAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UDonAttributeSet>("AttributeSet");

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("Health Bar Widget Component");
}

void ADonEnemyCharacter::Destroyed()
{
	if (ADonGameModeBase* GameModeBase = Cast<ADonGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameModeBase->AddToSpawnedEnemies(-1);
	}
	GetWorld()->GetTimerManager().ClearTimer(HealthVisibilityTimerHandle);
	
	Super::Destroyed();
}

void ADonEnemyCharacter::SetMeshInitState()
{

}

void ADonEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AddCharacterAbilities();
	
	
	if (ADonAIController* DonAIController = Cast<ADonAIController>(NewController))
	{
		DonAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		DonAIController->RunBehaviorTree(BehaviorTree);
	}
}

void ADonEnemyCharacter::Die_Implementation(const FVector& DeathImpulse, float ItemDropRate)
{
	Super::Die_Implementation(DeathImpulse, ItemDropRate);
	
	OnDungeonGroupKilled.Broadcast(GroupID);
	
	const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = GetActorRotation();
	FEnemyClassInfo EnemyClassInfo = UDonAbilityLibrary::FindEnemyClassInfo(this, EnemyClass);

	UDonItemLibrary::SpawnLootableXP(this, EnemyClassInfo.DroppableXP, SpawnLocation, SpawnRotation);
	UDonItemLibrary::SpawnLootableMoney(this, EnemyClassInfo.DroppableMoney, FMath::RandRange(0, 3), SpawnLocation, SpawnRotation);
	for (FItemLootableInfo& LootableItem : LootableItems)
	{
		// Normalized Rate
		float AdjustedRate = ItemDropRate * 0.01f * LootableItem.DropRate;
		UDonItemLibrary::SpawnLootableItem(this, LootableItems, SpawnLocation, SpawnRotation, AdjustedRate);
	}

	if (DeathEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathEffect, SpawnLocation);
	Destroy();
}

void ADonEnemyCharacter::SetHealthPercent(float NewValue)
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

void ADonEnemyCharacter::SetHealthText(float NewValue, float NewMaxValue)
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

void ADonEnemyCharacter::SetHealthVisibility(bool State)
{
	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(State);
	}
}

void ADonEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
	HealthBarComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

void ADonEnemyCharacter::InitAbilityActorInfo()
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
