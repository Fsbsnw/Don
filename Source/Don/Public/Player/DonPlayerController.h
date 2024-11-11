// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTags.h"
#include "DonPlayerController.generated.h"

class UDonInputConfig;
class USplineComponent;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class DON_API ADonPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADonPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OpenInventory();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputMappingContext* DefaultContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	TObjectPtr<UDonInputConfig> InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);


	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void CursorTrace();

	FHitResult HitResult;

	FVector CachedDestination = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = "Auto Run")
	float AutoRunThreshold = 0.5f;
	float ClickHoldTime = 0.f;
	bool bAutoRunning = false;
	
	void AutoRun();
};
