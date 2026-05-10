// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DonAbilityLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "DonAbilityTypes.h"
#include "DonGameplayTags.h"
#include "Character/Enemy/DonEnemyPawn.h"
#include "GameInstance/DonGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"
#include "UI/HUD/DonHUD.h"
#include "UI/WidgetController/DonWidgetController.h"


class UDonGameInstance;

UAttributeMenuWidgetController* UDonAbilityLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(PC->GetHUD()))
		{
			ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DonHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

FGameplayEffectContextHandle UDonAbilityLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FDonGameplayTags& GameplayTags = FDonGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	if (FDonGameplayEffectContext* DonEffectContext = static_cast<FDonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DonEffectContext->SetKnockbackForce(DamageEffectParams.KnockbackForce);
	}
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}


FCharacterClassInfo UDonAbilityLibrary::FindCharacterClassInfo(const UObject* WorldContextObject, ECharacterClass CharacterClass)
{
	UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance && DonGameInstance->CharacterClassDataTable)
	{
		UDataTable* CharacterDT = DonGameInstance->CharacterClassDataTable;
		TArray<FCharacterClassInfo*> CharacterClasses; 
		CharacterDT->GetAllRows(TEXT(""), CharacterClasses);
		for (FCharacterClassInfo* Character : CharacterClasses)
		{
			if (Character->CharacterClass == CharacterClass) return *Character;
		}
	}
	return FCharacterClassInfo();
}

FEnemyClassInfo UDonAbilityLibrary::FindEnemyClassInfo(const UObject* WorldContextObject, EEnemyClass EnemyClass)
{
	UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance && DonGameInstance->EnemyClassDataTable)
	{
		UDataTable* EnemyClassDT = DonGameInstance->EnemyClassDataTable;
		TArray<FEnemyClassInfo*> EnemyClasses; 
		EnemyClassDT->GetAllRows(TEXT(""), EnemyClasses);
		for (FEnemyClassInfo* Enemy : EnemyClasses)
		{
			if (Enemy->EnemyClass == EnemyClass) return *Enemy;
		}
	}
	return FEnemyClassInfo();
}

UAbilityInfo* UDonAbilityLibrary::GetAbilityInfo(const UObject* WorldContextObject, ECharacterClass CharacterClass)
{
	UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (DonGameInstance && DonGameInstance->AbilityInfo)
	{
		// Check Character Class
		
		return DonGameInstance->AbilityInfo;
	}
	return nullptr;
}
