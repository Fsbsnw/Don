// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractionComponent.generated.h"


class USphereComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> InteractionCollision;

public:
	void Interact();
};
