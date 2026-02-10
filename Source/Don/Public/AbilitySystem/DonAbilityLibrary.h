// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Character/DonCharacterBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DonAbilityLibrary.generated.h"

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
	UFUNCTION(BlueprintPure, Category = "DonInteractLibrary | Attribute")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static FCharacterClassInfo FindCharacterClassInfo(const UObject* WorldContextObject, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintPure, Category = "DonItemLibrary | Item")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category = "DonAbilitySystemLibrary | DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
};
