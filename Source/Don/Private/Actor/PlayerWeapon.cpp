// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PlayerWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "DonGameplayTags.h"
#include "AbilitySystem/DonAbilityLibrary.h"
#include "Components/SphereComponent.h"

APlayerWeapon::APlayerWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Axe = CreateDefaultSubobject<USkeletalMeshComponent>("Axe Mesh");
	Axe->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Axe);

	AxeCollision = CreateDefaultSubobject<USphereComponent>("Axe Collision");
	AxeCollision->SetupAttachment(Axe);
	AxeCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	AxeCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AxeCollision->SetGenerateOverlapEvents(false);
	AxeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	AxeCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWeaponBeginOverlap);
}

void APlayerWeapon::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(this)) return;
	if (!IsValid(OverlappedComponent)) return;
	if (!IsValid(OtherActor)) return;

	AActor* WeaponOwner = GetParentActor();
	if (!IsValid(WeaponOwner)) return;

	if (OtherActor == WeaponOwner) return;

	if (IgnoreActors.Contains(OtherActor)) return;
	IgnoreActors.Add(OtherActor);

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		FRotator Rotation = WeaponOwner->GetActorRotation();
		Rotation.Pitch = 20.f;
		
		const FVector DirForce = Rotation.Vector() * DamageEffectParams.KnockbackForceMagnitude;
		DamageEffectParams.KnockbackForce = DirForce;
		
		FGameplayEventData Payload;
		Payload.Instigator = WeaponOwner;
		Payload.Target = OtherActor;


		const float CurrentTime = GetWorld()->GetTimeSeconds();
		const float TimeDiff = CurrentTime - LastLightningTime;
		
		const FVector CurrentLocation = OtherActor->GetActorLocation();
		const float Distance = FVector::Dist(LastLightningLocation, CurrentLocation);

		// 번개 이펙트 발동 가능한 거리, 시간이 아닌 경우
		if (Distance < LightningThresholdDistance || TimeDiff < LightningThresholdTime)
		{
			Payload.EventMagnitude = -100.f;
		}
		// 번개 이펙트 발동 가능한 경우 시간, 위치 갱신
		else
		{
			LastLightningTime = CurrentTime;
			LastLightningLocation = CurrentLocation;
		}
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(WeaponOwner, FDonGameplayTags::Get().Request_Abilities_Lightning, Payload);
		
		UDonAbilityLibrary::ApplyDamageEffect(DamageEffectParams);
	}
}

void APlayerWeapon::UpdateAbilityTypeAndCollision(bool bEnableCollision)
{
	if (bEnableCollision)
	{
		AxeCollision->SetGenerateOverlapEvents(true);
		AxeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		AxeCollision->SetGenerateOverlapEvents(false);
		AxeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		IgnoreActors.Empty();
		DamageEffectParams = FDamageEffectParams();
	}
} 