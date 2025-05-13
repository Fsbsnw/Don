// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TeleportAttack.h"

#include "AbilitySystem/Abilities/AbilityTasks/AbilityTask_Timeline.h"
#include "Character/Enemy/DonEnemy.h"
#include "Kismet/KismetSystemLibrary.h"

bool UTeleportAttack::FindCombatTarget()
{
	TargetIndex = 0;
	ActorsToIgnore.Empty();
	OverlappedActors.Empty();
	
	AActor* Avatar = GetAvatarActorFromActorInfo();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	ActorsToIgnore.Add(Avatar);

	const bool IsTargetInRange = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Avatar->GetActorLocation(),
		CombatRadius,
		ObjectTypes,
		ADonEnemy::StaticClass(),
		ActorsToIgnore,
		OverlappedActors
	);

	if (!IsTargetInRange) return false;

	TeleportSpeed = OverlappedActors.Num() <= 3 ? 5.f : TeleportSpeed * OverlappedActors.Num();
	DelayMultiplier = OverlappedActors.Num() <= 3 ? 0.018f : (0.018f / OverlappedActors.Num()); 
	SetTeleportTarget();
	
	return true;
}

void UTeleportAttack::SetTeleportTarget()
{
	if (TargetIndex >= OverlappedActors.Num())
	{
		RequestEndAbility();
		return;
	}
	
	AActor* Avatar = GetAvatarActorFromActorInfo();

	StartLocation = Avatar->GetActorLocation();
	TargetLocation = OverlappedActors[TargetIndex]->GetActorLocation();
	const FRotator Direction = (TargetLocation - StartLocation).Rotation();

	FinalLocation = TargetLocation + (Direction.Vector() * OverDistance);

	UAbilityTask_Timeline* Timeline = UAbilityTask_Timeline::AbilityTaskOnTick(this, FName(""));
	Timeline->Multiplier = TeleportSpeed;
	Timeline->OnTick.AddDynamic(this, &UTeleportAttack::SetTransformToTarget);
	Timeline->ReadyForActivation();

	TargetIndex++;
	TeleportToTarget();
}

void UTeleportAttack::SetTransformToTarget(float Value)
{
	const FVector& NewLocation = FMath::Lerp(StartLocation, FinalLocation, Value);
	const FRotator& NewRotation = (TargetLocation - NewLocation).Rotation();
	GetAvatarActorFromActorInfo()->SetActorLocationAndRotation(NewLocation, NewRotation);

	if (Value > 1.f)
	{
		FTimerHandle TeleportTimer;
		GetWorld()->GetTimerManager().SetTimer(TeleportTimer, this, &UTeleportAttack::SetTeleportTarget, .2f);
	}
}
