// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DonPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DonGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "Character/Player/DonCharacter.h"
#include "Components/SplineComponent.h"
#include "Input/DonInputComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"
#include "UI/Widget/DamageTextComponent.h"

ADonPlayerController::ADonPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ADonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FString MapName = GetWorld()->GetMapName();
	if (MapName.Contains("Dungeon"))
	{
		FInputModeGameOnly Mode;
		this->SetInputMode(Mode);
		this->bShowMouseCursor = false;
	}
	else
	{
		FInputModeGameAndUI Mode;
		this->SetInputMode(Mode);
		this->bShowMouseCursor = true;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultContext, 0);
	}
}

void ADonPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void ADonPlayerController::ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bCriticalHit, false);
	}
}

void ADonPlayerController::ShowEvadeText(ACharacter* TargetCharacter, bool bEvade)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(0, false, bEvade);
	}
}

void ADonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UDonInputComponent* DonInputComponent = CastChecked<UDonInputComponent>(InputComponent);

	DonInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::OnMoveInput);
	}
}

void ADonPlayerController::OnMoveInput(const FInputActionValue& Value)
{
	APawn* ControlledPawn = GetPawn();

	const FVector2D Input = Value.Get<FVector2D>();
	if (Input.IsNearlyZero()) return;

	const FRotator ControlRot = GetControlRotation();
	const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

	const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector Right   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	ControlledPawn->AddMovementInput(Forward, Input.Y);
	ControlledPawn->AddMovementInput(Right,   Input.X);
}

void ADonPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed %s"), *InputTag.ToString());
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_RMB))
	{
		bAutoRunning = false;
		return;
	}
	
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void ADonPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_RMB))
	{		
		const APawn* ControlledPawn = GetPawn();
		if (ControlledPawn && InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_RMB))
		{
			if (ClickHoldTime < AutoRunThreshold)
			{
				if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
				{
					Spline->ClearSplinePoints();
					for (const FVector& PointLoc : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					}
					if (NavPath->PathPoints.Num() > 0)
					{
						CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
						bAutoRunning = true;
					}
				}
			}
			
			ClickHoldTime = 0.f;
		}
	}

	if (InputTag.MatchesTag(FDonGameplayTags::Get().UI))
	{
		OnUIOpenRequested.ExecuteIfBound(InputTag);
	}
	
	// j k q e esc

	// Interact with NPC
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_E))
	{
		if (ADonCharacter* DonCharacter = Cast<ADonCharacter>(GetPawn()))
		{
			DonCharacter->ExecuteInteract();
		}
	}

	// Use Quick Slot 1
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_NumKey_1))
	{
		ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
		if (DonPlayerState)
		{
			if (DonPlayerState->GetInventoryComponent()->FindQuickSlotItemByInputTag(InputTag))
			{
				DonPlayerState->GetInventoryComponent()->UseQuickSlotItem(InputTag);
			}
		}
	}

	// Use Quick Slot 2
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_NumKey_2))
	{
		ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
		if (DonPlayerState)
		{
			if (DonPlayerState->GetInventoryComponent()->FindQuickSlotItemByInputTag(InputTag))
			{
				DonPlayerState->GetInventoryComponent()->UseQuickSlotItem(InputTag);
			}
		}
	}

	// Use Quick Slot 3
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_NumKey_3))
	{
		ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
		if (DonPlayerState)
		{
			if (DonPlayerState->GetInventoryComponent()->FindQuickSlotItemByInputTag(InputTag))
			{
				DonPlayerState->GetInventoryComponent()->UseQuickSlotItem(InputTag);
			}
		}
	}

	// Use Quick Slot 4
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_NumKey_4))
	{
		ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
		if (DonPlayerState)
		{
			if (DonPlayerState->GetInventoryComponent()->FindQuickSlotItemByInputTag(InputTag))
			{
				DonPlayerState->GetInventoryComponent()->UseQuickSlotItem(InputTag);
			}
		}
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
}

void ADonPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_RMB))
	{
		ClickHoldTime += GetWorld()->GetDeltaSeconds();

		if (HitResult.bBlockingHit) CachedDestination = HitResult.ImpactPoint;

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector Direction = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(Direction);
		}
	}

	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
}

void ADonPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
}

void ADonPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	
	if (APawn* ControlledPawn = GetPawn())
	{		
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);
		
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

UDonAbilitySystemComponent* ADonPlayerController::GetASC()
{
	if (DonAbilitySystemComponent == nullptr)
	{
		DonAbilitySystemComponent = Cast<UDonAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return DonAbilitySystemComponent;
}