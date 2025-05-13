// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DonSweepingAttack.h"

#include "Character/Enemy/DonEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDonSweepingAttack::CauseDamage(AActor* TargetActor)
{
	Super::CauseDamage(TargetActor);

	const FVector OwnerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();

	FRotator Rotation = (TargetLocation - OwnerLocation).Rotation();
	Rotation.Pitch = Pitch;

	if (ADonEnemy* DonEnemy = Cast<ADonEnemy>(TargetActor))
	{
		DonEnemy->GetCharacterMovement()->AddImpulse((Rotation.Vector() * Force * 100.f));
	}
}
