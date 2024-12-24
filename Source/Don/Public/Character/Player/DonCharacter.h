// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DonCharacterBase.h"
#include "DonCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class DON_API ADonCharacter : public ADonCharacterBase
{
	GENERATED_BODY()
public:
	ADonCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Interact();
	
protected:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(BlueprintCallable)
	bool ExecuteInteract(AActor* Actor);

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;

private:
};
