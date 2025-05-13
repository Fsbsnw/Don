// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DonProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Don.h"
#include "DonAbilityTypes.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Inventory/DonItemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ADonProjectile::ADonProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ADonProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADonProjectile::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void ADonProjectile::OnHit()
{
	if (bCriticalHit) UGameplayStatics::PlaySoundAtLocation(this, ImpactCriticalSound, GetActorLocation(), FRotator::ZeroRotator);
	else UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}

void ADonProjectile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (!bHit) OnHit();
	Super::Destroyed();
}

void ADonProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (bHit) return;
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
		DamageEffectParams.DeathImpulse = DeathImpulse;
		const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
		if (bKnockback)
		{
			FRotator Rotation = GetActorRotation();
			Rotation.Pitch = 45.f;
			
			const FVector KnockbackDirection = Rotation.Vector();
			const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
			DamageEffectParams.KnockbackForce = KnockbackForce;
		}
		
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		FGameplayEffectContextHandle ContextHandle = UDonItemLibrary::ApplyDamageEffect(DamageEffectParams);
		if (const FDonGameplayEffectContext* EffectContext = static_cast<const FDonGameplayEffectContext*>(ContextHandle.Get()))
		{
			bCriticalHit = EffectContext->GetIsCriticalHit();
			OnHit();
		}
	}
	
	Destroy();
	bHit = true;
}

bool ADonProjectile::IsValidOverlap(AActor* OtherActor)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;

	return true;
}