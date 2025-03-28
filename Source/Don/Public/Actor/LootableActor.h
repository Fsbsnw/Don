// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LootableActor.generated.h"

/**
 * 
 */
UCLASS()
class DON_API ALootableActor : public AActor
{
	GENERATED_BODY()
public:
	ALootableActor();
	void InitializeSpawnLocation();
	void PreventLooting();
	void EnableCollision();
	
protected:
	virtual void BeginPlay() override;
	void LerpToSpawnOffsetLocation(float DeltaSeconds);
	void LerpToPlayer(float DeltaSeconds);
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	void HandleLoot(AActor* TargetActor);

	// Follow Player
	UPROPERTY()
	AActor* TargetPlayer = nullptr;
	bool bFollowingPlayer = false;
	FVector FollowingTargetLocation;
	float FollowingTime = 0.f;
	UPROPERTY(EditAnywhere)
	float FollowingTimeLimit = 1.5f;

	// Move to Offset Location
	FVector SpawnLocation;
	FVector SpawnOffsetLocation;
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 1.f;
	
	UPROPERTY(EditAnywhere)
	float MaxSpawnOffset = 150.f;
	UPROPERTY(EditAnywhere)
	float MinSpawnOffset = 75.f;
	
	float OffsetTime = 0.f;
	UPROPERTY(EditAnywhere)
	float OffsetMultiplier = 2.f;
	UPROPERTY(EditAnywhere)
	float OffsetTimeLimit = 0.75f;
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag LootType;
	
	UPROPERTY(EditAnywhere)
	int32 LootAmount = 0;

	UPROPERTY(EditAnywhere)
	FName ItemName;

};
