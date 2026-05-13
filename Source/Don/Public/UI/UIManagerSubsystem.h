// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/UIConfigDataAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManagerSubsystem.generated.h"

struct FInteractionWidgetContext;
class UDonUserWidget;

/**
 * 
 */
UCLASS()
class DON_API UUIManagerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void InitializeUIConfig(AGameStateBase* GameState);
	void InitializeUIBinding(APlayerController* PC);

	void OnToggleRequested(FGameplayTag InputTag);	
	void ToggleWidget(FGameplayTag Tag);
	void OpenWidget(FGameplayTag Tag);
	void OpenInteractionWidget(FGameplayTag Tag, const FInteractionWidgetContext& IWC);

private:
	TMap<FGameplayTag, FUIConfigWidget> ConfigData;

	UPROPERTY()
	TMap<FGameplayTag, UDonUserWidget*> ActiveWidgets;
};
