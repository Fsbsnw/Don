// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DonAbilityTypes.h"
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
	void Die_Implementation(const FVector& DeathImpulse, float ItemDropRate) override;
	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameOver();

	UFUNCTION()
	void UpdateAttributesFromLevel(int32 NewLevel, bool bLevelUp);

	UPROPERTY(EditAnywhere, Category = "Test")
	bool bCanDead = true;

	// Begin IPlayerInterface 

	virtual int32 GetAttributePoints_Implementation() const override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToMoney_Implementation(int32 InMoney) override;
	virtual void AddToScore_Implementation(int32 InScore) override;
	virtual bool AddItemToInventory_Implementation(FItem Item) override;

	// End IPlayerInterface

	UFUNCTION(BlueprintCallable)
	void UpdateAbilityTypeAndCollision(FGameplayTag AbilityTag, bool bEnableCollision);
	void ExecuteInteract();
	
protected:
	virtual void InitAbilityActorInfo() override;


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> InteractionCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> Axe;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> AxeCollision;

	FGameplayTag AxeAbilityType;
	
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	TArray<AActor*> IgnoreActors;
private:
};