// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/DonCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DonGameplayTags.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/DonDamageGameplayAbility.h"
#include "Camera/CameraComponent.h"
#include "Character/Interface/InteractInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerController.h"
#include "Player/DonPlayerState.h"
#include "UI/HUD/DonHUD.h"

ADonCharacter::ADonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	Axe = CreateDefaultSubobject<USkeletalMeshComponent>("Axe Mesh");
	Axe->SetupAttachment(GetMesh(), "ik_hand_gun");
	Axe->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AxeCollision = CreateDefaultSubobject<USphereComponent>("Axe Collision");
	AxeCollision->SetupAttachment(Axe);
	AxeCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	AxeCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AxeCollision->SetGenerateOverlapEvents(false);
	AxeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractionCollision = CreateDefaultSubobject<USphereComponent>("Interaction Collision");
	InteractionCollision->SetSphereRadius(100.f);
	InteractionCollision->SetupAttachment(RootComponent);
	InteractionCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	InteractionCollision->SetGenerateOverlapEvents(true);
	InteractionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ADonCharacter::BeginPlay()
{
	Super::BeginPlay();

	AxeCollision->OnComponentBeginOverlap.AddDynamic(this, &ADonCharacter::OnWeaponBeginOverlap);
}

void ADonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void ADonCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ADonCharacter::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;
	if (IgnoreActors.Contains(OtherActor)) return;

	IgnoreActors.Add(OtherActor);

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		FRotator Rotation = GetActorRotation();
		Rotation.Pitch = 20.f;
		
		const FVector DirForce = Rotation.Vector() * DamageEffectParams.KnockbackForceMagnitude;
		DamageEffectParams.KnockbackForce = DirForce;
		
		FGameplayEventData Payload;
		Payload.Instigator = this;
		Payload.Target = OtherActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FDonGameplayTags::Get().Request_Abilities_Lightning, Payload);
		
		UDonItemLibrary::ApplyDamageEffect(DamageEffectParams);
	}
}

void ADonCharacter::Die_Implementation(const FVector& DeathImpulse, float ItemDropRate)
{
	Super::Die_Implementation(DeathImpulse, ItemDropRate);

	if (bCanDead) ShowGameOver();
}

void ADonCharacter::UpdateAttributesFromLevel(int32 NewLevel, bool bLevelUp)
{
	if (!bLevelUp) return;

	GetAbilitySystemComponent()->SetActiveGameplayEffectLevel(MaxVitalEffectHandle, NewLevel);
	ApplyEffectToSelf(DefaultVitalAttributes, NewLevel);
}

void ADonCharacter::UpdateAbilityTypeAndCollision(FGameplayTag AbilityTag, bool bEnableCollision)
{
	if (!AxeCollision) return;

	if (bEnableCollision && AbilityTag.IsValid())
	{
		AxeCollision->SetGenerateOverlapEvents(true);
		AxeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AxeAbilityType = AbilityTag;
	}
	else
	{
		AxeCollision->SetGenerateOverlapEvents(false);
		AxeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AxeAbilityType = FGameplayTag();
		IgnoreActors.Empty();
		DamageEffectParams = FDamageEffectParams();
	}
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

void ADonCharacter::InitAbilityActorInfo()
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	check(DonPlayerState);
	DonPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DonPlayerState, this);
	DonPlayerState->OnLevelChangedDelegate.AddUObject(this, &ADonCharacter::UpdateAttributesFromLevel);

	Cast<UDonAbilitySystemComponent>(DonPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	AbilitySystemComponent = DonPlayerState->GetAbilitySystemComponent();
	AttributeSet = DonPlayerState->GetAttributeSet();

	if (ADonPlayerController* DonPlayerController = Cast<ADonPlayerController>(GetController()))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(DonPlayerController->GetHUD()))
		{
			DonHUD->InitOverlay(DonPlayerController, DonPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}

void ADonCharacter::ExecuteInteract()
{
	TArray<AActor*> OverlappingActors;
	InteractionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* NPCActor : OverlappingActors)
	{
		if (NPCActor->Implements<UInteractInterface>())
		{
			Cast<IInteractInterface>(NPCActor)->Interact(GetPlayerState());
			return;
		}
	}
}