// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DonPlayerController.h"

#include "DonGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Character/Player/DonCharacter.h"
#include "Components/SplineComponent.h"
#include "Input/DonInputComponent.h"

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
						DrawDebugSphere(GetWorld(), PointLoc, 50.f, 12, FColor::Red, false, 4.f);
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

	// Open Inventory
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_Tab))
	{
		OpenInventory();
	}

	// Interact with NPC
	if (InputTag.MatchesTagExact(FDonGameplayTags::Get().InputTag_E))
	{
		if (ADonCharacter* DonCharacter = Cast<ADonCharacter>(GetPawn()))
		{
			DonCharacter->Interact();
		}
	}
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
