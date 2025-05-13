// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LootableActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DonGameplayTags.h"
#include "Character/Player/Interface/PlayerInterface.h"
#include "Inventory/DonItemLibrary.h"

ALootableActor::ALootableActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALootableActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeSpawnLocation();
	PreventLooting();
}

void ALootableActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LerpToSpawnOffsetLocation(DeltaSeconds);
	LerpToPlayer(DeltaSeconds);

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += RotationSpeed * DeltaSeconds; 
	SetActorRotation(NewRotation);
}

void ALootableActor::InitializeSpawnLocation()
{
	SpawnLocation = GetActorLocation();

	FVector OffsetLocation;

	// Set X Vector
	if (FMath::RandBool())
	{
		OffsetLocation.X = SpawnLocation.X + FMath::RandRange(-MaxSpawnOffset, -MinSpawnOffset);
	}
	else
	{
		OffsetLocation.X = SpawnLocation.X + FMath::RandRange(MinSpawnOffset, MaxSpawnOffset);
	}

	// Set Y Vector
	if (FMath::RandBool())
	{
		OffsetLocation.Y = SpawnLocation.Y + FMath::RandRange(-MaxSpawnOffset, -MinSpawnOffset);
	}
	else
	{
		OffsetLocation.Y = SpawnLocation.Y + FMath::RandRange(MinSpawnOffset, MaxSpawnOffset);
	}
	OffsetLocation.Z = SpawnLocation.Z;
	SpawnOffsetLocation = OffsetLocation;
}

void ALootableActor::PreventLooting()
{
	SetActorEnableCollision(false);	

	FTimerHandle LootHandler;
	GetWorldTimerManager().SetTimer(LootHandler, this, &ALootableActor::EnableCollision, 1.f);
}

void ALootableActor::LerpToSpawnOffsetLocation(float DeltaSeconds)
{
	// Before Collision Enabled
	if (OffsetTime < OffsetTimeLimit)
	{
		OffsetTime += DeltaSeconds * OffsetMultiplier;
		FVector NewLocation = FMath::Lerp(SpawnLocation, SpawnOffsetLocation, FMath::Clamp(OffsetTime, 0.f, 1.f));
		
		SetActorLocation(NewLocation);
	}
}

void ALootableActor::LerpToPlayer(float DeltaSeconds)
{
	// After Collision Enabled, Overlapped with player
	if (bFollowingPlayer && TargetPlayer && FollowingTime < FollowingTimeLimit)
	{
		FollowingTime += DeltaSeconds;
		FollowingTargetLocation = TargetPlayer->GetActorLocation();
		FVector NewLocation = FMath::Lerp(SpawnOffsetLocation, FollowingTargetLocation, FollowingTime / 1.5f);

		SetActorLocation(NewLocation);
		
		if (FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation()) < 50.f)
		{
			if (!TargetPlayer->Implements<UPlayerInterface>()) return;
			
			FDonGameplayTags GameplayTags = FDonGameplayTags::Get();
	
			if (LootType.MatchesTag(GameplayTags.Item))
			{
				FItem ItemToAdd = UDonItemLibrary::FindItemByName(this, ItemName);
				if (!IPlayerInterface::Execute_AddItemToInventory(TargetPlayer, ItemToAdd))
				{
					bFollowingPlayer = false;
					TargetPlayer = nullptr;
					FollowingTime = 0.f;
					return;
				}
			}
			else if (LootType.MatchesTag(GameplayTags.Attributes_Meta_IncomingXP) || LootType.MatchesTag(GameplayTags.Attributes_Meta_IncomingMoney))
			{
				FGameplayEventData Payload;
				Payload.EventTag = LootType;
				Payload.EventMagnitude = LootAmount;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetPlayer, LootType, Payload);
			}

			Destroy();
		}
	}
}

void ALootableActor::EnableCollision()
{
	SetActorEnableCollision(true);
}

void ALootableActor::HandleLoot(AActor* TargetActor)
{
	if (!TargetActor->Implements<UPlayerInterface>() || bFollowingPlayer) return;

	bFollowingPlayer = true;
	TargetPlayer = TargetActor;
}