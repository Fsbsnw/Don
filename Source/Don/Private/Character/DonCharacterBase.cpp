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
		const float AttackPower = Weapon->GetFinalAttribute();
		Weapon->RequestDestroy();
		Weapon = nullptr;
		AddBonusAttackPower(-AttackPower);
	}
}

void ADonCharacterBase::UnequipArmorHelmet_Implementation()
{
	if (ArmorHelmet)
	{
		const float Defense = ArmorHelmet->GetFinalAttribute();
		ArmorHelmet->Destroy();
		ArmorHelmet = nullptr;
		AddBonusDefense(-Defense);
	}
}

void ADonCharacterBase::UnequipArmorChest_Implementation()
{
	if (ArmorChest)
	{
		const float Defense = ArmorChest->GetFinalAttribute();
		ArmorChest->Destroy();
		ArmorChest = nullptr;
		AddBonusDefense(-Defense);
	}
}

void ADonCharacterBase::UnequipArmorHands_Implementation()
{
	float Defense = 0.f;
	if (ArmorLeftHand)
	{
		Defense = ArmorLeftHand->GetFinalAttribute() * 2;
		ArmorLeftHand->Destroy();
	}
	ArmorLeftHand = nullptr;

	if (ArmorRightHand)
	{
		ArmorRightHand->Destroy();
	}
	ArmorRightHand = nullptr;
	AddBonusDefense(-Defense);
}

void ADonCharacterBase::UnequipArmorLegs_Implementation()
{
	if (ArmorLegs) 
	{
		const float Defense = ArmorLegs->GetFinalAttribute();
		ArmorLegs->Destroy();
		ArmorLegs = nullptr;
		AddBonusDefense(-Defense);
	}
}

void ADonCharacterBase::UnequipArmorBoots_Implementation()
{
	float Defense = 0.f;
	if (ArmorLeftBoot) 
	{
		Defense = ArmorLeftBoot->GetFinalAttribute() * 2;
		ArmorLeftBoot->Destroy();
	}
	ArmorLeftBoot = nullptr;

	if (ArmorRightBoot)
	{
		ArmorRightBoot->Destroy();
	}
	ArmorRightBoot = nullptr;
	AddBonusDefense(-Defense);
}

void ADonCharacterBase::EquipItem_Implementation(FItem& Item)
{
	FDonGameplayTags GameplayTags = FDonGameplayTags::Get();
	if (Item.ItemTag.MatchesTagExact(GameplayTags.Item_Equippable_Armor_Helmet))
	{
		EquipArmorHelmet_Implementation(Item);
	}
	else if (Item.ItemTag == GameplayTags.Item_Equippable_Armor_Chest)
	{
		EquipArmorChest_Implementation(Item);
	}
	else if (Item.ItemTag == GameplayTags.Item_Equippable_Armor_Hands)
	{
		EquipArmorHands_Implementation(Item);
	}
	else if (Item.ItemTag == GameplayTags.Item_Equippable_Armor_Legs)
	{
		EquipArmorLegs_Implementation(Item);
	}
	else if (Item.ItemTag == GameplayTags.Item_Equippable_Armor_Boots)
	{
		EquipArmorBoots_Implementation(Item);
	}
}

void ADonCharacterBase::EquipArmorHelmet_Implementation(FItem& Item)
{
	if (ArmorHelmet && Item.IsSameInstance(ArmorHelmet->GetEquipmentInfo()))
	{
		UnequipArmorHelmet_Implementation();
		return;
	}
	FItemEquipmentInfo EquipmentInfo = UDonItemLibrary::FindItemEquipmentByName(this, Item.ItemName);
	if (EquipmentInfo.ItemActorClass == nullptr) return;

	if (UClass* LoadedClass = EquipmentInfo.ItemActorClass.LoadSynchronous())
	{
		if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(LoadedClass))
		{
			Equipment->InitEquipmentAttributes();
			Equipment->SetEquipmentInfo(Item);
			ArmorHelmet = Equipment;
			Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorHelmetSocket);

			const float Defense = ArmorHelmet->GetFinalAttribute();
			AddBonusDefense(Defense);
		}
	}
}

void ADonCharacterBase::EquipArmorChest_Implementation(FItem& Item)
{
	if (ArmorChest && Item.IsSameInstance(ArmorChest->GetEquipmentInfo()))
	{
		UnequipArmorChest_Implementation();
		return;
	}
	FItemEquipmentInfo EquipmentInfo = UDonItemLibrary::FindItemEquipmentByName(this, Item.ItemName);
	if (EquipmentInfo.ItemActorClass == nullptr) return;
	
	if (UClass* LoadedClass = EquipmentInfo.ItemActorClass.LoadSynchronous())
	{
		if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(LoadedClass))
		{
			Equipment->InitEquipmentAttributes();
			Equipment->SetEquipmentInfo(Item);
			ArmorChest = Equipment;
			Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorChestSocket);

			const float Defense = ArmorChest->GetFinalAttribute();
			AddBonusDefense(Defense);
		}
	}
}

void ADonCharacterBase::EquipArmorHands_Implementation(FItem& Item)
{
	if (ArmorLeftHand || ArmorRightHand && Item.IsSameInstance(ArmorLeftHand->GetEquipmentInfo()))
	{
		UnequipArmorHands_Implementation();
		return;
	}
	float Defense = 0.f;
	FItemEquipmentInfo EquipmentInfo = UDonItemLibrary::FindItemEquipmentByName(this, Item.ItemName);
	if (EquipmentInfo.ItemActorClass == nullptr) return;
	
	if (UClass* LoadedClass = EquipmentInfo.ItemActorClass.LoadSynchronous())
	{
		if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(LoadedClass))
		{
			Equipment->InitEquipmentAttributes();
			Equipment->SetEquipmentInfo(Item);
			ArmorLeftHand = Equipment;
			Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorLeftHandSocket);

			Defense = ArmorLeftHand->GetFinalAttribute() * 2;
		}
	}
	if (UClass* LoadedClass = EquipmentInfo.ItemActorClass.LoadSynchronous())
	{
		if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(LoadedClass))
		{
			Equipment->InitEquipmentAttributes();
			Equipment->SetEquipmentInfo(Item);
			ArmorRightHand = Equipment;
			Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorRightHandSocket);
		}
	}
	AddBonusDefense(Defense);
}

void ADonCharacterBase::EquipArmorLegs_Implementation(FItem& Item)
{
	if (ArmorLegs && Item.IsSameInstance(ArmorLegs->GetEquipmentInfo()))
	{
		UnequipArmorLegs_Implementation();
		return;
	}
	FItemEquipmentInfo EquipmentInfo = UDonItemLibrary::FindItemEquipmentByName(this, Item.ItemName);
	if (EquipmentInfo.ItemActorClass == nullptr) return;
	
	if (UClass* LoadedClass = EquipmentInfo.ItemActorClass.LoadSynchronous())
	{
		if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(LoadedClass))
		{
			Equipment->InitEquipmentAttributes();
			Equipment->SetEquipmentInfo(Item);
			ArmorLegs = Equipment;
			Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorLegsSocket);

			const float Defense = ArmorLegs->GetFinalAttribute();
			AddBonusDefense(Defense);
		}
	}
}

void ADonCharacterBase::EquipArmorBoots_Implementation(FItem& Item)
{
	if (ArmorLeftBoot || ArmorRightBoot && Item.IsSameInstance(ArmorLeftBoot->GetEquipmentInfo()))
	{
		UnequipArmorBoots_Implementation();
		return;
	}
	float Defense = 0.f;
	FItemEquipmentInfo EquipmentInfo = UDonItemLibrary::FindItemEquipmentByName(this, Item.ItemName);
	if (EquipmentInfo.ItemActorClass == nullptr) return;
	
	if (UClass* LoadedClass = EquipmentInfo.ItemActorClass.LoadSynchronous())
	{
		if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(LoadedClass))
		{
			Equipment->InitEquipmentAttributes();
			Equipment->SetEquipmentInfo(Item);
			ArmorLeftBoot = Equipment;
			Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorLeftBootSocket);

			Defense = ArmorLeftBoot->GetFinalAttribute();
		}
	}
	if (UClass* LoadedClass = EquipmentInfo.ItemActorClass.LoadSynchronous())
	{
		if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(LoadedClass))
		{
			Equipment->InitEquipmentAttributes();
			Equipment->SetEquipmentInfo(Item);
			ArmorRightBoot = Equipment;
			Equipment->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ArmorRightBootSocket);
		}
	}
	AddBonusDefense(Defense * 2);
}

void ADonCharacterBase::UpdateUpgradedItemInfo_Implementation(const FItem& Item)
{
	if (ArmorHelmet && Item.IsSameInstance(ArmorHelmet->GetEquipmentInfo()))
	{
		UpdateUpgradedArmorPoint(ArmorHelmet, Item.EquipmentAttribute.Upgrade);
	}
	else if (ArmorChest && Item.IsSameInstance(ArmorChest->GetEquipmentInfo()))
	{
		UpdateUpgradedArmorPoint(ArmorChest, Item.EquipmentAttribute.Upgrade);
	}
	else if (ArmorLegs && Item.IsSameInstance(ArmorLegs->GetEquipmentInfo()))
	{
		UpdateUpgradedArmorPoint(ArmorLegs, Item.EquipmentAttribute.Upgrade);
	}
	else if (ArmorLeftBoot && Item.IsSameInstance(ArmorLeftBoot->GetEquipmentInfo()))
	{
		UpdateUpgradedArmorPoint(ArmorLeftBoot, Item.EquipmentAttribute.Upgrade);
		UpdateUpgradedArmorPoint(ArmorRightBoot, Item.EquipmentAttribute.Upgrade);
	}
	else if (ArmorLeftHand && Item.IsSameInstance(ArmorLeftHand->GetEquipmentInfo()))
	{
		UpdateUpgradedArmorPoint(ArmorLeftHand, Item.EquipmentAttribute.Upgrade);
		UpdateUpgradedArmorPoint(ArmorRightHand, Item.EquipmentAttribute.Upgrade);
	}
}


float ADonCharacterBase::GetWeaponDamage_Implementation()
{
	if (const ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(GetPlayerState()))
	{
		return DonPlayerState->GetAxeUpgrade() * 3.f;
	}
	return 0.f;
}

float ADonCharacterBase::GetArmorDefense_Implementation()
{
	float FinalDefense = 0.f;
	if (ArmorHelmet) FinalDefense += ArmorHelmet->GetFinalAttribute();
	if (ArmorChest) FinalDefense += ArmorChest->GetFinalAttribute();
	if (ArmorLegs) FinalDefense += ArmorLegs->GetFinalAttribute();
	if (ArmorLeftHand) FinalDefense += ArmorLeftHand->GetFinalAttribute() * 2.f;
	if (ArmorLeftBoot) FinalDefense += ArmorLeftBoot->GetFinalAttribute() * 2.f;

	return FinalDefense;
}

int32 ADonCharacterBase::GetEquippedArmorCount_Implementation()
{
	int32 Count = 0;
	if (ArmorHelmet) Count++;
	if (ArmorChest) Count++;
	if (ArmorLegs) Count++;
	if (ArmorLeftHand) Count++;
	if (ArmorLeftBoot) Count++;
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
