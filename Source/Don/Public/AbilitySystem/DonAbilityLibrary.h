// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Character/DonCharacterTypes.h"
#include "Character/DonPawnBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DonAbilityLibrary.generated.h"

struct FEnemyClassInfo;
enum class EEnemyClass;
struct FDamageEffectParams;
class UAbilityInfo;
class UAttributeMenuWidgetController;
/**
 * 
 */
UCLASS()
class DON_API UDonAbilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "DonItemLibrary")
	static FCharacterClassInfo FindCharacterClassInfo(const UObject* WorldContextObject, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary")
	static FEnemyClassInfo FindEnemyClassInfo(const UObject* WorldContextObject, EEnemyClass EnemyClass);

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category = "DonAbilitySystemLibrary | DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
};
