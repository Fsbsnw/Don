// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class UInteractComponent;

UINTERFACE(MinimalAPI, BlueprintType)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DON_API IInteractInterface
{
	GENERATED_BODY()

public:
	virtual UInteractComponent* GetInteractComponent() = 0;
	virtual void Interact(APlayerState* PlayerState) = 0;
};
