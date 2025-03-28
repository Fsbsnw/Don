// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DonAbilityTypes.h"
#include "AbilitySystem/Abilities/DonDamageGameplayAbility.h"
#include "Character/DonCharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "Engine/EngineTypes.h"
#include "DonCharacter.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class DON_API ADonCharacter : public ADonCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	ADonCharacter();

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	// Begin IPlayerInterface 

	virtual int32 GetAttributePoints_Implementation() const override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToMoney_Implementation(int32 InMoney) override;
	virtual bool AddItemToInventory_Implementation(FItem Item) override;

	// End IPlayerInterface
	
	UFUNCTION(BlueprintImplementableEvent)
	void Interact();

	UFUNCTION(BlueprintCallable)
	void UpdateAbilityTypeAndCollision(FGameplayTag AbilityTag, bool bEnableCollision);
protected:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(BlueprintCallable)
	bool ExecuteInteract(AActor* Actor);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> Axe;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> AxeCollision;

	FGameplayTag AxeAbilityType;
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(EditAnywhere)
	float LightningDamage = 0.f;

	TArray<AActor*> IgnoreActors;
private:
};