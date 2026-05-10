// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DonPawnBase.h"

#include "AbilitySystemComponent.h"
#include "Don.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/DonPlayerState.h"

ADonPawnBase::ADonPawnBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComponent);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(CapsuleComponent);

	CapsuleComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CapsuleComponent->SetGenerateOverlapEvents(false);

	SkeletalMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	SkeletalMesh->SetGenerateOverlapEvents(true);
}

UAbilitySystemComponent* ADonPawnBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADonPawnBase::Die_Implementation(const FVector& DeathImpulse, float ItemDropRate)
{
	if (!bDead)
	{
		bDead = true;
	}
}

void ADonPawnBase::SetKnockbackState_Implementation(bool NewState, const FVector& Force)
{
	CapsuleComponent->AddImpulse(Force);
}

float ADonPawnBase::GetCharacterLevel_Implementation() const
{
	if (const ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetPlayerState()))
	{
		return DonPlayerState->GetPlayerLevel();
	}
	return 0.f;
}

void ADonPawnBase::BeginPlay()
{
	Super::BeginPlay();

	if (SkeletalMesh && !SkeletalMesh->GetMaterials().IsEmpty() && DynamicMaterials.IsEmpty())
	{
		for (int32 Index = 0; Index < SkeletalMesh->GetMaterials().Num(); Index++)
		{
			UMaterialInstanceDynamic* InstanceDynamic = UMaterialInstanceDynamic::Create(SkeletalMesh->GetMaterials()[Index], this);
			SkeletalMesh->SetMaterial(Index, InstanceDynamic);
			DynamicMaterials.AddUnique(InstanceDynamic);
		}
	}
}

void ADonPawnBase::ApplyHitEffect_Implementation()
{
	if (!DynamicMaterials.IsEmpty())
	{
		for (UMaterialInstanceDynamic* Material : DynamicMaterials)
		{
			Material->SetVectorParameterValue(FName("HitFlashColor"), FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		}
		GetWorld()->GetTimerManager().SetTimer(HitFlashTimerHandle, this, &ADonPawnBase::ResetMaterials, 0.2f, false);
	}
}

void ADonPawnBase::ResetMaterials()
{
	if (!DynamicMaterials.IsEmpty())
	{
		for (UMaterialInstanceDynamic* Material : DynamicMaterials)
		{
			Material->SetVectorParameterValue(FName("HitFlashColor"), FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)); // 초기화
		}
	}
}

void ADonPawnBase::InitAbilityActorInfo()
{
}

FActiveGameplayEffectHandle ADonPawnBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	return GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ADonPawnBase::InitializeDefaultAttributes()
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, GetCharacterLevel_Implementation());
	SecondaryEffectHandle = ApplyEffectToSelf(DefaultSecondaryAttributes, GetCharacterLevel_Implementation());
	MaxVitalEffectHandle = ApplyEffectToSelf(DefaultMaxVitalAttributes, GetCharacterLevel_Implementation());
	ApplyEffectToSelf(DefaultVitalAttributes, GetCharacterLevel_Implementation());
}

void ADonPawnBase::AddCharacterAbilities()
{
	UDonAbilitySystemComponent* ASC = CastChecked<UDonAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	ASC->AddCharacterAbilities(StartupAbilities);
	ASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
	ASC->AddCharacterStartupAbilities(StartupCommonAbilities);
}