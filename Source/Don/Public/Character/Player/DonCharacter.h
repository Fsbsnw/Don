// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DonAbilityTypes.h"
#include "Character/DonCharacterBase.h"
#include "Character/DonCharacterTypes.h"
#include "Interface/PlayerInterface.h"
#include "Engine/EngineTypes.h"
#include "DonCharacter.generated.h"

class APlayerWeapon;
class UInteractionComponent;
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

	virtual void Die_Implementation(const FVector& DeathImpulse, float ItemDropRate) override;
	virtual void EquipItem_Implementation(FItem& Item) override;
	virtual void UnequipItem_Implementation(FItem& Item) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameOver();

	UFUNCTION()
	void UpdateAttributesFromLevel(int32 NewLevel, bool bLevelUp);

	UPROPERTY(EditAnywhere, Category = "Test")
	bool bCanDead = true;

	ECharacterClass CharacterClass = ECharacterClass::Fighter;

	// Begin IPlayerInterface 

	virtual int32 GetAttributePoints_Implementation() const override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToMoney_Implementation(int32 InMoney) override;
	virtual void AddToScore_Implementation(int32 InScore) override;
	virtual bool AddItemToInventory_Implementation(FItem Item) override;

	// End IPlayerInterface
	
protected:
	virtual void InitAbilityActorInfo() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UChildActorComponent> PlayerWeapon;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> InteractionCollision;

	UFUNCTION(BlueprintCallable)
	APlayerWeapon* GetPlayerWeapon();
private:
	void OnMoveSpeedChanged(const FOnAttributeChangeData& Data);
	
};