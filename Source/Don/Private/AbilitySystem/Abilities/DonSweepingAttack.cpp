// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DonSweepingAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Inventory/DonItemLibrary.h"

void UDonSweepingAttack::CauseDamage(AActor* TargetActor)
{
	if (TargetActor == GetAvatarActorFromActorInfo()) return;

	// Super::CauseDamage(TargetActor);
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		const FVector OwnerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();

		FRotator Rotation = (TargetLocation - OwnerLocation).Rotation();
		Rotation.Pitch = Pitch;

		DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		DamageEffectParams.KnockbackForceMagnitude = Force;
		DamageEffectParams.KnockbackForce = Rotation.Vector() * DamageEffectParams.KnockbackForceMagnitude * 100.f;
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		
		FGameplayEffectContextHandle ContextHandle = UDonItemLibrary::ApplyDamageEffect(DamageEffectParams);
	}	
}
