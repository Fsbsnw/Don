// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class UInteractComponent;

enum class EInteractionType : uint8
{
	PickupItem		UMETA(DisplayName = "PickupItem"),
	StartDialogue	UMETA(DisplayName = "StartDialogue"),
	OpenStore		UMETA(DisplayName = "OpenStore"),
};

USTRUCT(BlueprintType)
struct FInteractionWidgetContext
{
	GENERATED_BODY()

	EInteractionType InteractionType = EInteractionType::PickupItem;

	FGameplayTag WidgetTag = FGameplayTag();

	UPROPERTY()
	AActor* InteractionTarget = nullptr;
};

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
	UFUNCTION(BlueprintNativeEvent)
	FInteractionWidgetContext Interact();
};
