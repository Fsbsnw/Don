// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DonProjectileSpell.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h"

void UDonProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

ADonProjectile* UDonProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride)
{
	const FVector SocketLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	ADonProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADonProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	
	if (bArcType && TargetActor != nullptr)
	{
		const FVector Start = GetAvatarActorFromActorInfo()->GetActorLocation();
		const FVector Target = TargetActor->GetActorLocation();

		const FVector ToTarget = Target - Start;

		// 수평 방향 벡터 (Z 제외)
		FVector ToTargetXY = FVector(ToTarget.X, ToTarget.Y, 0.f);
		FVector DirXY = ToTargetXY.GetSafeNormal();

		const float Distance = ToTargetXY.Size(); // 수평 거리 R
		const float Gravity = 980.f * Projectile->ProjectileMovement->ProjectileGravityScale;
		const float AngleRadians = FMath::DegreesToRadians(AngleDegrees);

		const float Sin2Theta = FMath::Sin(2 * AngleRadians);
		const float VSquared = (Distance * Gravity) / Sin2Theta;
		const float V = FMath::Sqrt(VSquared);

		// 방향 벡터 계산
		const FVector Velocity = V * (FMath::Cos(AngleRadians) * DirXY + FMath::Sin(AngleRadians) * FVector::UpVector);
		
		Projectile->ProjectileMovement->Velocity = Velocity;
		Projectile->ProjectileMovement->InitialSpeed = V;
		Projectile->ProjectileMovement->MaxSpeed = V;

		UE_LOG(LogTemp, Warning, TEXT("Distance : %f, Gravity : %f, AngleRadians : %f, VSquared : %f, V : %f, Velocity : %s"), Distance, Gravity, AngleRadians, VSquared, V, *Velocity.ToString());
	}
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
	Projectile->FinishSpawning(SpawnTransform);
	return Projectile;
}
