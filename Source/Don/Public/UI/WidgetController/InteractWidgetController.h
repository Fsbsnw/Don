// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractWidgetController.generated.h"

struct FDialogue;
class UInteractComponent;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DON_API UInteractWidgetController : public UObject
{
	GENERATED_BODY()
public:
	void SetWidgetControllerParams(UInteractComponent* InInteractComponent, APlayerState* TargetPlayerState);

	UFUNCTION(BlueprintCallable)
	FDialogue SetCurrentDialogueProgress();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UInteractComponent> InteractComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;
};
