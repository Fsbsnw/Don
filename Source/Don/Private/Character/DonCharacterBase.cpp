// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DonCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "Don.h"
#include "DonGameplayTags.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "Actor/DonEquipmentActor.h"
#include "Components/CapsuleComponent.h"
#include "Data/ItemAsset.h"

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

void ADonCharacterBase::Die_Implementation(const FVector& DeathImpulse)
{
	if (!bDead)
	{
		bDead = true;
		UE_LOG(LogTemp, Warning, TEXT("%s is Dead"), *GetName());
	}
}

void ADonCharacterBase::SetKnockbackState_Implementation(bool NewState, const FVector& Force)
{
	
}

bool ADonCharacterBase::IsItemEquipped_Implementation(FItem& Item)
{
	UE_LOG(LogTemp, Warning, TEXT("Checking if item '%s' is equipped."), *Item.ItemName.ToString());

	if (Weapon && Item.ItemName == Weapon->EquipmentName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item '%s' matches Weapon: '%s'"), *Item.ItemName.ToString(), *Weapon->EquipmentName.ToString());
		return true;
	}
	if (ArmorHelmet && Item.ItemName == ArmorHelmet->EquipmentName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item '%s' matches ArmorHelmet: '%s'"), *Item.ItemName.ToString(), *ArmorHelmet->EquipmentName.ToString());
		return true;
	}
	if (ArmorChest && Item.ItemName == ArmorChest->EquipmentName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item '%s' matches ArmorChest: '%s'"), *Item.ItemName.ToString(), *ArmorChest->EquipmentName.ToString());
		return true;
	}
	if (ArmorRightHand && Item.ItemName == ArmorRightHand->EquipmentName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item '%s' matches ArmorRightHand: '%s'"), *Item.ItemName.ToString(), *ArmorRightHand->EquipmentName.ToString());
		return true;
	}
	if (ArmorLegs && Item.ItemName == ArmorLegs->EquipmentName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item '%s' matches ArmorLegs: '%s'"), *Item.ItemName.ToString(), *ArmorLegs->EquipmentName.ToString());
		return true;
	}
	if (ArmorRightBoot && Item.ItemName == ArmorRightBoot->EquipmentName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item '%s' matches ArmorRightBoot: '%s'"), *Item.ItemName.ToString(), *ArmorRightBoot->EquipmentName.ToString());
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Item '%s' is NOT equipped."), *Item.ItemName.ToString());
	return false;
	// return (
	// 	(Weapon && Item.ItemName == Weapon->EquipmentName) ||
	// 	(ArmorHelmet && Item.ItemName == ArmorHelmet->EquipmentName) ||
	// 	(ArmorChest && Item.ItemName == ArmorChest->EquipmentName) ||
	// 	(ArmorRightHand && Item.ItemName == ArmorRightHand->EquipmentName) ||
	// 	(ArmorLegs && Item.ItemName == ArmorLegs->EquipmentName) ||
	// 	(ArmorRightBoot && Item.ItemName == ArmorRightBoot->EquipmentName)
	// );
}

void ADonCharacterBase::UnequipAllItems_Implementation()
{
	ICombatInterface::Execute_UnequipWeapon(this);
	ICombatInterface::Execute_UnequipArmorHelmet(this);
	ICombatInterface::Execute_UnequipArmorChest(this);
	ICombatInterface::Execute_UnequipArmorHands(this);
	ICombatInterface::Execute_UnequipArmorLegs(this);
	ICombatInterface::Execute_UnequipArmorBoots(this);
}

void ADonCharacterBase::UnequipItem_Implementation(FItem& Item)
{
	const FDonGameplayTags& Tag = FDonGameplayTags::Get();
	if (Item.ItemTag.MatchesTagExact(Tag.Item_Equippable_Weapon))
	{
		UnequipWeapon_Implementation();
	}
	else if (Item.ItemTag.MatchesTagExact(Tag.Item_Equippable_Armor_Helmet))
	{
		UnequipArmorHelmet_Implementation();
	}
	else if (Item.ItemTag.MatchesTagExact(Tag.Item_Equippable_Armor_Chest))
	{
		UnequipArmorChest_Implementation();
	}
	else if (Item.ItemTag.MatchesTagExact(Tag.Item_Equippable_Armor_Hands))
	{
		UnequipArmorHands_Implementation();
	}
	else if (Item.ItemTag.MatchesTagExact(Tag.Item_Equippable_Armor_Legs))
	{
		UnequipArmorLegs_Implementation();
	}
	else if (Item.ItemTag.MatchesTagExact(Tag.Item_Equippable_Armor_Boots))
	{
		UnequipArmorBoots_Implementation();
	}
}


void ADonCharacterBase::UnequipWeapon_Implementation()
{
	if (Weapon)
	{
		Weapon->RequestDestroy();
		Weapon = nullptr;
	}
}

void ADonCharacterBase::UnequipArmorHelmet_Implementation()
{
	if (ArmorHelmet) ArmorHelmet->Destroy();
	ArmorHelmet = nullptr;
}

void ADonCharacterBase::UnequipArmorChest_Implementation()
{
	if (ArmorChest) ArmorChest->Destroy();
	ArmorChest = nullptr;
}

void ADonCharacterBase::UnequipArmorHands_Implementation()
{
	if (ArmorLeftHand) ArmorLeftHand->Destroy();
	ArmorLeftHand = nullptr;

	if (ArmorRightHand) ArmorRightHand->Destroy();
	ArmorRightHand = nullptr;
}

void ADonCharacterBase::UnequipArmorLegs_Implementation()
{
	if (ArmorLegs) ArmorLegs->Destroy();
	ArmorLegs = nullptr;
}

void ADonCharacterBase::UnequipArmorBoots_Implementation()
{
	if (ArmorLeftBoot) ArmorLeftBoot->Destroy();
	ArmorLeftBoot = nullptr;

	if (ArmorRightBoot) ArmorRightBoot->Destroy();
	ArmorRightBoot = nullptr;
}

void ADonCharacterBase::EquipWeapon_Implementation(FItem& Item)
{
	if (Weapon)
	{
		UnequipWeapon_Implementation();
		return;
	}
	if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(Item.ItemActorClass))
	{
		Equipment->InitEquipmentAttributes();
		Weapon = Equipment;
		Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
	}
}

void ADonCharacterBase::EquipArmorHelmet_Implementation(FItem& Item)
{
	if (ArmorHelmet)
	{
		UnequipArmorHelmet_Implementation();
		return;
	}
	if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(Item.ItemActorClass))
	{
		Equipment->InitEquipmentAttributes();
		ArmorHelmet = Equipment;
		Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorHelmetSocket);
	}
}

void ADonCharacterBase::EquipArmorChest_Implementation(FItem& Item)
{
	if (ArmorChest)
	{
		UnequipArmorChest_Implementation();
		return;
	}
	if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(Item.ItemActorClass))
	{
		Equipment->InitEquipmentAttributes();
		ArmorChest = Equipment;
		Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorChestSocket);
	}
}

void ADonCharacterBase::EquipArmorHands_Implementation(FItem& Item)
{
	if (ArmorLeftHand || ArmorRightHand)
	{
		UnequipArmorHands_Implementation();
		return;
	}
	if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(Item.ItemActorClass))
	{
		Equipment->InitEquipmentAttributes();
		ArmorLeftHand = Equipment;
		Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorLeftHandSocket);
	}
	if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(Item.ItemActorClass))
	{
		Equipment->InitEquipmentAttributes();
		ArmorRightHand = Equipment;
		Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorRightHandSocket);
	}
}

void ADonCharacterBase::EquipArmorLegs_Implementation(FItem& Item)
{
	if (ArmorLegs)
	{
		UnequipArmorLegs_Implementation();
		return;
	}
	if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(Item.ItemActorClass))
	{
		Equipment->InitEquipmentAttributes();
		ArmorLegs = Equipment;
		Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorLegsSocket);
	}
}

void ADonCharacterBase::EquipArmorBoots_Implementation(FItem& Item)
{
	if (ArmorLeftBoot || ArmorRightBoot)
	{
		UnequipArmorBoots_Implementation();
		return;
	}
	if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(Item.ItemActorClass))
	{
		Equipment->InitEquipmentAttributes();
		ArmorLeftBoot = Equipment;
		Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorLeftBootSocket);
	}
	if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(Item.ItemActorClass))
	{
		Equipment->InitEquipmentAttributes();
		ArmorRightBoot = Equipment;
		Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorRightBootSocket);
	}
}

float ADonCharacterBase::GetWeaponDamage_Implementation()
{
	if (Weapon) return Weapon->GetFinalAttribute();
	return 0.f;
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

void ADonCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, 1.0f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ADonCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultMaxVitalAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ADonCharacterBase::AddCharacterAbilities()
{
	UDonAbilitySystemComponent* ASC = CastChecked<UDonAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	ASC->AddCharacterAbilities(StartupAbilities);
	ASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}
