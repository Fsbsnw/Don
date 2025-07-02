// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DonPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DonGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/DonCharacter.h"
#include "Components/SplineComponent.h"
#include "Input/DonInputComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"
#include "UI/HUD/DonHUD.h"
#include "UI/Widget/DamageTextComponent.h"

ADonPlayerController::ADonPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ADonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	this->bShowMouseCursor = true;

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

}

void ADonPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_RMB))
	{
		bAutoRunning = false;
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

	// Open Menu
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_ESC))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(GetHUD()))
		{
			DonHUD->OpenMenu();
		}		
	}

	
	// Open Inventory
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_Tab))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(GetHUD()))
		{
			DonHUD->OpenInventory();
		}		
	}

	// Open Attribute Menu
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_J))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(GetHUD()))
		{
			DonHUD->OpenAttributeMenu();
		}		
	}

	
	// Open Skill Menu
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_K))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(GetHUD()))
		{
			DonHUD->OpenSkillMenu();
		}		
	}

	// Open Quests
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_Q))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(GetHUD()))
		{
			DonHUD->OpenQuests();
		}		
	}

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