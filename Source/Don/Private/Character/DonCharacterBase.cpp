// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DonCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "Don.h"
#include "DonGameplayTags.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "Actor/DonEquipmentActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"
#include "Data/ItemStructs.h"
#include "Inventory/DonItemLibrary.h"

ADonCharacterBase::ADonCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAbilitySystemComponent* ADonCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADonCharacterBase::Die_Implementation(const FVector& DeathImpulse, float ItemDropRate)
{
	if (!bDead)
	{
		bDead = true;
		if (ActorHasTag(FName("Enemy")) && DeathSound)
		{
			UGameplayStatics::PlaySound2D(this, DeathSound, 1);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s is Dead"), *GetName());
	}
}

void ADonCharacterBase::SetKnockbackState_Implementation(bool NewState, const FVector& Force)
{
	GetCharacterMovement()->AddImpulse(Force);
}

bool ADonCharacterBase::IsItemEquipped_Implementation(FItem& Item)
{
	return EquipmentParts.Contains(Item.ItemTag);
}

void ADonCharacterBase::EquipItem_Implementation(FItem& Item)
{
	const  FDonGameplayTags& DonTags = FDonGameplayTags::Get();
	
	if (Item.ItemTag.MatchesTagExact(DonTags.Item_Equippable_Armor_Hands))
	{
		SpawnAndAttachEquipment(DonTags.Item_Equippable_Armor_Hands_Left, Item);
		SpawnAndAttachEquipment(DonTags.Item_Equippable_Armor_Hands_Right, Item);
	}
	else if (Item.ItemTag.MatchesTagExact(DonTags.Item_Equippable_Armor_Boots))
	{
		SpawnAndAttachEquipment(DonTags.Item_Equippable_Armor_Boots_Left, Item);
		SpawnAndAttachEquipment(DonTags.Item_Equippable_Armor_Boots_Right, Item);
	}
	else
	{
		SpawnAndAttachEquipment(Item.ItemTag, Item);
	}
}

void ADonCharacterBase::UnequipItem_Implementation(FItem& Item)
{
	const FDonGameplayTags& DonTags = FDonGameplayTags::Get();

	if (Item.ItemTag.MatchesTagExact(DonTags.Item_Equippable_Armor_Hands))
	{
		DetachAndDestroyEquipment(DonTags.Item_Equippable_Armor_Hands_Left);
		DetachAndDestroyEquipment(DonTags.Item_Equippable_Armor_Hands_Right);
	}
	else if (Item.ItemTag.MatchesTagExact(DonTags.Item_Equippable_Armor_Boots))
	{
		DetachAndDestroyEquipment(DonTags.Item_Equippable_Armor_Boots_Left);
		DetachAndDestroyEquipment(DonTags.Item_Equippable_Armor_Boots_Right);
	}
	else
	{
		DetachAndDestroyEquipment(Item.ItemTag);
	}
}

void ADonCharacterBase::SpawnAndAttachEquipment(const FGameplayTag& SlotTag, const FItem& Item)
{
	// 기존 장비 제거
	if (EquipmentParts.Contains(SlotTag))
	{
		ADonEquipmentActor* Existing = EquipmentParts[SlotTag];
		if (Existing)
		{
			if (Item.IsSameInstance(Existing->GetEquipmentInfo()))
			{
				DetachAndDestroyEquipment(SlotTag);
				return;
			}
			DetachAndDestroyEquipment(SlotTag);
		}
	}
	
	FItemEquipmentInfo EquipmentInfo = UDonItemLibrary::FindItemEquipmentByName(this, Item.ItemName);
	if (UClass* LoadedClass = EquipmentInfo.ItemActorClass.LoadSynchronous())
	{
		if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(LoadedClass))
		{
			Equipment->InitEquipmentAttributes();
			Equipment->SetEquipmentInfo(Item);

			EquipmentParts.Add(SlotTag, Equipment);

			// 장비 속성 추가
			
			Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, EquipmentSockets[SlotTag]);

			OnEquipmentChanged.Broadcast();
		}
	}
}

void ADonCharacterBase::DetachAndDestroyEquipment(const FGameplayTag& SlotTag)
{
	if (EquipmentParts.Contains(SlotTag))
	{
		ADonEquipmentActor* Existing = EquipmentParts[SlotTag];
		if (Existing)
		{
			Existing->Destroy();
			EquipmentParts.Remove(SlotTag);
			OnEquipmentChanged.Broadcast();
		}
	}
}

void ADonCharacterBase::UpdateUpgradedItemInfo_Implementation(const FItem& Item)
{
	// if (ArmorHelmet && Item.IsSameInstance(ArmorHelmet->GetEquipmentInfo()))
	// {
	// 	UpdateUpgradedArmorPoint(ArmorHelmet, Item.EquipmentAttribute.Upgrade);
	// }
	// else if (ArmorChest && Item.IsSameInstance(ArmorChest->GetEquipmentInfo()))
	// {
	// 	UpdateUpgradedArmorPoint(ArmorChest, Item.EquipmentAttribute.Upgrade);
	// }
	// else if (ArmorLegs && Item.IsSameInstance(ArmorLegs->GetEquipmentInfo()))
	// {
	// 	UpdateUpgradedArmorPoint(ArmorLegs, Item.EquipmentAttribute.Upgrade);
	// }
	// else if (ArmorLeftBoot && Item.IsSameInstance(ArmorLeftBoot->GetEquipmentInfo()))
	// {
	// 	UpdateUpgradedArmorPoint(ArmorLeftBoot, Item.EquipmentAttribute.Upgrade);
	// 	UpdateUpgradedArmorPoint(ArmorRightBoot, Item.EquipmentAttribute.Upgrade);
	// }
	// else if (ArmorLeftHand && Item.IsSameInstance(ArmorLeftHand->GetEquipmentInfo()))
	// {
	// 	UpdateUpgradedArmorPoint(ArmorLeftHand, Item.EquipmentAttribute.Upgrade);
	// 	UpdateUpgradedArmorPoint(ArmorRightHand, Item.EquipmentAttribute.Upgrade);
	// }
}

float ADonCharacterBase::GetWeaponDamage_Implementation()
{
	if (const ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetPlayerState()))
	{
		return DonPlayerState->GetAxeUpgrade() * 3.f;
	}
	return 0.f;
}

int32 ADonCharacterBase::GetEquippedArmorCount_Implementation()
{
	int32 Count = 0;
	const FDonGameplayTags& DonTags = FDonGameplayTags::Get();
	if (EquipmentParts.Contains(DonTags.Item_Equippable_Armor_Helmet)) Count++;
	if (EquipmentParts.Contains(DonTags.Item_Equippable_Armor_Chest)) Count++;
	if (EquipmentParts.Contains(DonTags.Item_Equippable_Armor_Hands_Left)) Count++;
	if (EquipmentParts.Contains(DonTags.Item_Equippable_Armor_Legs)) Count++;
	if (EquipmentParts.Contains(DonTags.Item_Equippable_Armor_Boots_Left)) Count++;
	return Count;
}

int32 ADonCharacterBase::GetRewardScore_Implementation()
{
	return RewardScore;
}

float ADonCharacterBase::GetCharacterLevel_Implementation() const
{
	if (const ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetPlayerState()))
	{
		return DonPlayerState->GetPlayerLevel();
	}
	else
	{
		return CharacterLevel;
	}
}

float ADonCharacterBase::GetArmorDefense_Implementation()
{
	float Defense = 0.f;
	for (TTuple<FGameplayTag, TObjectPtr<ADonEquipmentActor>> EquipmentInfo : EquipmentParts)
	{
		Defense += EquipmentInfo.Value->GetFinalAttribute();
	}
	return Defense;
}

void ADonCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetMesh() && !GetMesh()->GetMaterials().IsEmpty() && DynamicMaterials.IsEmpty())
	{
		for (int32 Index = 0; Index < GetMesh()->GetMaterials().Num(); Index++)
		{
			UMaterialInstanceDynamic* InstanceDynamic = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterials()[Index], this);
			GetMesh()->SetMaterial(Index, InstanceDynamic);
			DynamicMaterials.AddUnique(InstanceDynamic);
		}
	}
}

void ADonCharacterBase::ApplyHitEffect_Implementation()
{
	if (!DynamicMaterials.IsEmpty())
	{
		for (UMaterialInstanceDynamic* Material : DynamicMaterials)
		{
			Material->SetVectorParameterValue(FName("HitFlashColor"), FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		}
		GetWorld()->GetTimerManager().SetTimer(HitFlashTimerHandle, this, &ADonCharacterBase::ResetMaterials, 0.2f, false);
	}
}

void ADonCharacterBase::ResetMaterials()
{
	if (!DynamicMaterials.IsEmpty())
	{
		for (UMaterialInstanceDynamic* Material : DynamicMaterials)
		{
			Material->SetVectorParameterValue(FName("HitFlashColor"), FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)); // 초기화
		}
	}
}

void ADonCharacterBase::InitAbilityActorInfo()
{
}

FActiveGameplayEffectHandle ADonCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	return GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ADonCharacterBase::InitializeDefaultAttributes()
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, GetCharacterLevel_Implementation());
	SecondaryEffectHandle = ApplyEffectToSelf(DefaultSecondaryAttributes, GetCharacterLevel_Implementation());
	MaxVitalEffectHandle = ApplyEffectToSelf(DefaultMaxVitalAttributes, GetCharacterLevel_Implementation());
	ApplyEffectToSelf(DefaultVitalAttributes, GetCharacterLevel_Implementation());
}

void ADonCharacterBase::AddCharacterAbilities()
{
	UDonAbilitySystemComponent* ASC = CastChecked<UDonAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	ASC->AddCharacterAbilities(StartupAbilities);
	ASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
	ASC->AddCharacterStartupAbilities(StartupCommonAbilities);
}

void ADonCharacterBase::AddBonusAttackPower(int32 AttackPower)
{
	BonusAttackPower = FMath::Max(0, BonusAttackPower + AttackPower);
	OnEquipmentChanged.Broadcast();
}

void ADonCharacterBase::AddBonusDefense(int32 Defense)
{
	BonusDefense = FMath::Max(0, BonusDefense + Defense);
	OnEquipmentChanged.Broadcast();
}

void ADonCharacterBase::UpdateUpgradedArmorPoint(ADonEquipmentActor* Armor, int32 InPoint)
{
	if (Armor == nullptr) return;

	Armor->UpdateUpgradedPoint(InPoint);
}
