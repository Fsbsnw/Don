// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Interface/InteractInterface.h"
#include "GameFramework/Character.h"
#include "NPCCharacterBase.generated.h"

UCLASS()
class DON_API ANPCCharacterBase : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:
	ANPCCharacterBase();

	virtual UInteractComponent* GetInteractComponent() override;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact(APlayerState* TargetPlayerState) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	FString CharacterName;
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UInteractComponent> InteractComponent;
};
