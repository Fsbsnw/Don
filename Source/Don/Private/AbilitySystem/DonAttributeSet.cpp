// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DonAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "DonAbilityTypes.h"
#include "DonGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/DonEnemy.h"
#include "Character/Player/DonCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerController.h"

UDonAttributeSet::UDonAttributeSet()
{
	const FDonGameplayTags& Tags = FDonGameplayTags::Get();
	
	TagsToAttributes.Add(Tags.Attributes_Primary_Vigor, GetVigorAttribute);
	TagsToAttributes.Add(Tags.Attributes_Primary_Agility, GetAgilityAttribute);
	TagsToAttributes.Add(Tags.Attributes_Primary_Supernatural, GetSupernaturalAttribute);

	TagsToAttributes.Add(Tags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(Tags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(Tags.Attributes_Secondary_DodgeChance, GetDodgeChanceAttribute);
	TagsToAttributes.Add(Tags.Attributes_Secondary_SkillCooldown, GetSkillCooldownAttribute);
	TagsToAttributes.Add(Tags.Attributes_Secondary_SpawnChance, GetSpawnChanceAttribute);
	TagsToAttributes.Add(Tags.Attributes_Secondary_ExpGainRate, GetExpGainRateAttribute);
	TagsToAttributes.Add(Tags.Attributes_Secondary_MoneyGainRate, GetMoneyGainRateAttribute);
	TagsToAttributes.Add(Tags.Attributes_Secondary_ItemDropRate, GetItemDropRateAttribute);
}

void UDonAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDonAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDonAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDonAttributeSet, MoneyGainRate, COND_None, REPNOTIFY_Always);
}

void UDonAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UDonAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (!IsValid(Props.TargetCharacter)) return;
	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;
	
	FGameplayAttribute Attribute = Data.EvaluatedData.Attribute;
	
	if (Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("OwningActor : %s, Health : %f"), *GetOwningActor()->GetName(), GetHealth());
	}
	if (Attribute == GetIncomingXPAttribute() && GetIncomingXP() != 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f XP"), GetIncomingXP());
		const int32 LocalXP = GetIncomingXP() * (GetExpGainRate() * 0.01f);
		SetIncomingXP(0);

		if (Props.SourceCharacter->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalXP);
		}
	}
	if (Attribute == GetIncomingMoneyAttribute() && GetIncomingMoney() != 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f Money"), GetIncomingMoney());
		const int32 LocalMoney = GetIncomingMoney() * (GetMoneyGainRate() * 0.01f);
		SetIncomingMoney(0);

		if (Props.SourceCharacter->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToMoney(Props.SourceCharacter, LocalMoney);
		}
	}
	if (Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			if (FMath::RandRange(1, 100) <= GetDodgeChance())
			{
				ShowEvadeText(Props, true);
				return;
			}
				
			if (Props.TargetCharacter->Implements<UCombatInterface>())
			{
				ICombatInterface::Execute_ApplyHitEffect(Props.TargetCharacter);
			}

			int32 WeaponDamage = 0.f;
			int32 ArmorDefense = 0.f;
			if (Props.SourceCharacter->Implements<UCombatInterface>() && Props.TargetCharacter->Implements<UCombatInterface>())
			{
				WeaponDamage = FMath::RoundToInt(ICombatInterface::Execute_GetWeaponDamage(Props.SourceCharacter));
				ArmorDefense = FMath::RoundToInt(ICombatInterface::Execute_GetArmorDefense(Props.TargetCharacter));
				UE_LOG(LogTemp, Warning, TEXT("%s's Weapon Damage Added : %d Damaged"), *Props.SourceCharacter->GetName(), WeaponDamage);
				UE_LOG(LogTemp, Warning, TEXT("%s's Armor Defense Added : %d Blocked"), *Props.TargetCharacter->GetName(), ArmorDefense);
			}
			int32 FinalDamage = FMath::Max(LocalIncomingDamage + WeaponDamage - ArmorDefense, 0.f);
			const float NewHealth = GetHealth() - FinalDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal && Props.TargetCharacter->Implements<UCombatInterface>())
			{
				int32 RewardScore = ICombatInterface::Execute_GetRewardScore(Props.TargetCharacter);
				bool bFound = true;
				float DropRate = Props.SourceASC->GetGameplayAttributeValue(GetItemDropRateAttribute(), bFound);
				ICombatInterface::Execute_Die(Props.TargetCharacter, FVector(), DropRate);
				if (Props.SourceCharacter->Implements<UPlayerInterface>())
				{
					IPlayerInterface::Execute_AddToScore(Props.SourceCharacter, RewardScore);
				}
			}

			if (Props.EffectContextHandle.IsValid() && Props.EffectContextHandle.Get()) 
			{
				if (const FDonGameplayEffectContext* DonEffectContext = static_cast<const FDonGameplayEffectContext*>(Props.EffectContextHandle.Get()))
				{
					ShowFloatingText(Props, FinalDamage, DonEffectContext->GetIsCriticalHit());
					

					if (AAIController* AIController = Cast<AAIController>(Props.TargetCharacter->GetController()))
					{
						if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
						{
							BlackboardComp->SetValueAsBool(TEXT("bIsHitReacting"), true);
						}
					}

					const FVector& KnockbackForce = DonEffectContext->GetKnockbackForce();
					if (!KnockbackForce.IsNearlyZero())
					{
						Props.TargetCharacter->StopAnimMontage();
						ICombatInterface::Execute_SetKnockbackState(Props.TargetCharacter, true, KnockbackForce);
						if (Props.TargetCharacter->Implements<UPlayerInterface>())
						{
							Props.TargetASC->TryActivateAbilitiesByTag(FDonGameplayTags::Get().Effects_HitReact.GetSingleTagContainer());
						}
					}
					else
					{
						if (ADonEnemy* Enemy = Cast<ADonEnemy>(Props.TargetCharacter))
						{
							if (!Enemy->GetGetupState()) return;
						}
						Props.TargetASC->TryActivateAbilitiesByTag(FDonGameplayTags::Get().Effects_HitReact.GetSingleTagContainer());
					}
				}
			}
		}
	}
}

void UDonAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bCriticalHit) const
{
	if (!IsValid(Props.SourceCharacter) || !IsValid(Props.TargetCharacter)) return;
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if(ADonPlayerController* PC = Cast<ADonPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bCriticalHit);
			return;
		}
		if(ADonPlayerController* PC = Cast<ADonPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bCriticalHit);
		}
	}
}

void UDonAttributeSet::ShowEvadeText(const FEffectProperties& Props, bool bEvade) const
{
	if (!IsValid(Props.SourceCharacter) || !IsValid(Props.TargetCharacter)) return;
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if(ADonPlayerController* PC = Cast<ADonPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowEvadeText(Props.TargetCharacter, bEvade);
			return;
		}
		if(ADonPlayerController* PC = Cast<ADonPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowEvadeText(Props.TargetCharacter, bEvade);
		}
	}
}

void UDonAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, Health, OldHealth);
}

void UDonAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, MaxHealth, OldMaxHealth);
}

void UDonAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, Vigor, OldVigor);
}

void UDonAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, Agility, OldAgility);
}

void UDonAttributeSet::OnRep_Supernatural(const FGameplayAttributeData& OldSupernatural) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, Supernatural, OldSupernatural);
}

void UDonAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UDonAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UDonAttributeSet::OnRep_DodgeChance(const FGameplayAttributeData& OldDodgeChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, DodgeChance, OldDodgeChance);
}

void UDonAttributeSet::OnRep_SkillCooldown(const FGameplayAttributeData& OldSkillCooldown) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, SkillCooldown, OldSkillCooldown);
}

void UDonAttributeSet::OnRep_SpawnChance(const FGameplayAttributeData& OldSpawnChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, SpawnChance, OldSpawnChance);
}

void UDonAttributeSet::OnRep_ItemDropRate(const FGameplayAttributeData& OldItemDropRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, ItemDropRate, OldItemDropRate);
}

void UDonAttributeSet::OnRep_MoneyGainRate(const FGameplayAttributeData& OldMoneyGainRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, MoneyGainRate, OldMoneyGainRate);
}

void UDonAttributeSet::OnRep_ExpGainRate(const FGameplayAttributeData& OldExpGainRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDonAttributeSet, ExpGainRate, OldExpGainRate);
}

void UDonAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}