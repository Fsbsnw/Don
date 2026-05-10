// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DonAbilityTypes.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "PlayerWeapon.generated.h"

class USphereComponent;

UCLASS()
class DON_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerWeapon();
	
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<AActor*> IgnoreActors;
	
	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateAbilityTypeAndCollision(bool bEnableCollision);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> Axe;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> AxeCollision;
	
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(EditAnywhere)
	float LightningThresholdTime = 0.05f;
	float LastLightningTime = 0.f;

	UPROPERTY(EditAnywhere)
	float LightningThresholdDistance = 50.f;
	FVector LastLightningLocation = FVector::Zero();
};
