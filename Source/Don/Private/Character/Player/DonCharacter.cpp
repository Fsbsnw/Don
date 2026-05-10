// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/DonCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DonGameModeBase.h"
#include "DonInnGameMode.h"
#include "AbilitySystem/DonAttributeSet.h"
#include "Actor/PlayerWeapon.h"
#include "Camera/CameraComponent.h"
#include "Character/Component/EquipmentComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"

ADonCharacter::ADonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	PlayerWeapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Player Weapon"));
	PlayerWeapon->SetupAttachment(GetMesh(), TEXT("ik_hand_gun"));
}

void ADonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerWeapon* Weapon = Cast<APlayerWeapon>(PlayerWeapon->GetChildActor()))
	{
		Weapon->SetOwner(this);
	}

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

// GAS 세팅 및 Default Abilities 적용 
void ADonCharacter::InitAbilityActorInfo()
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	check(DonPlayerState);
	
	AbilitySystemComponent = DonPlayerState->GetAbilitySystemComponent();
	check(AbilitySystemComponent);

	AttributeSet = DonPlayerState->GetAttributeSet();
	
	AbilitySystemComponent->InitAbilityActorInfo(DonPlayerState, this);
	DonPlayerState->OnLevelChangedDelegate.AddUObject(this, &ADonCharacter::UpdateAttributesFromLevel);

	// 캐릭터 Speed Attribute 바인딩
	if (UDonAttributeSet* DonAS = Cast<UDonAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			DonAS->GetMoveSpeedAttribute()
		).AddUObject(this, &ADonCharacter::OnMoveSpeedChanged);
	}

	// Curve Table 기반 Attributes 속성 적용(Primary, Secondary, MaxVital, Vital)
	InitializeDefaultAttributes();
}

APlayerWeapon* ADonCharacter::GetPlayerWeapon()
{
	if (AActor* Child = PlayerWeapon->GetChildActor())
	{
		if (APlayerWeapon* Weapon = Cast<APlayerWeapon>(Child))
		{
			return Weapon;
		}
	}
	return nullptr;
}

// Attribute-캐릭터 스피드 바인딩
void ADonCharacter::OnMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
	UE_LOG(LogTemp, Warning, TEXT("Move Speed : %f"), Data.NewValue);
}

void ADonCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ADonCharacter::Die_Implementation(const FVector& DeathImpulse, float ItemDropRate)
{
	Super::Die_Implementation(DeathImpulse, ItemDropRate);

	if (bCanDead)
	{
		if (ADonGameModeBase* GameMode = Cast<ADonGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->GameOver(0);
		}
		else
		{
			ShowGameOver();
		}
	}
}

void ADonCharacter::EquipItem_Implementation(FItem& Item)
{
	if (UEquipmentComponent* EquipComponent = FindComponentByClass<UEquipmentComponent>())
	{
		EquipComponent->SpawnAndAttachEquipment(Item);
	}
}

void ADonCharacter::UnequipItem_Implementation(FItem& Item)
{
	if (UEquipmentComponent* EquipComponent = FindComponentByClass<UEquipmentComponent>())
	{
		EquipComponent->DetachAndDestroyEquipment(Item);
	}
}

void ADonCharacter::UpdateAttributesFromLevel(int32 NewLevel, bool bLevelUp)
{
	if (!bLevelUp) return;

	GetAbilitySystemComponent()->SetActiveGameplayEffectLevel(MaxVitalEffectHandle, NewLevel);
	ApplyEffectToSelf(DefaultVitalAttributes, NewLevel);
}

int32 ADonCharacter::GetAttributePoints_Implementation() const
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	check(DonPlayerState);
	return DonPlayerState->GetAttributePoints();
}

void ADonCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	check(DonPlayerState);
	DonPlayerState->AddToAttributePoints(InAttributePoints);
}

void ADonCharacter::AddToXP_Implementation(int32 InXP)
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	check(DonPlayerState);
	DonPlayerState->AddToXP(InXP);
}

void ADonCharacter::AddToMoney_Implementation(int32 InMoney)
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	check(DonPlayerState);
	DonPlayerState->AddToMoney(InMoney);
}

void ADonCharacter::AddToScore_Implementation(int32 InScore)
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	check(DonPlayerState);
	DonPlayerState->AddToScore(InScore);
	DonPlayerState->AddToKillCount(1);
}

bool ADonCharacter::AddItemToInventory_Implementation(FItem Item)
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	check(DonPlayerState);
	DonPlayerState->GetInventoryComponent()->AddItem(Item, 1);
	return true;
}

