// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTags.h"
#include "DonPlayerController.generated.h"


struct FInputActionValue;
class UInputAction;
DECLARE_DELEGATE_OneParam(FOnUIOpenRequested, FGameplayTag);

class UDamageTextComponent;
class UDonAbilitySystemComponent;
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
	void InitializeHUD();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION()
	void ShowDamageNumber(float DamageAmount, APawn* TargetPawn, bool bCriticalHit);
	UFUNCTION()
	void ShowEvadeText(APawn* TargetPawn, bool bEvade);

	FOnUIOpenRequested OnUIOpenRequested;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputMappingContext* DefaultContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	TObjectPtr<UDonInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	TObjectPtr<UInputAction> IA_Move;

	void OnMoveInput(const FInputActionValue& Value);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UDonAbilitySystemComponent* GetASC();
	
	UPROPERTY()
	TObjectPtr<UDonAbilitySystemComponent> DonAbilitySystemComponent;
	
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};