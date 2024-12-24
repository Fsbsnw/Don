// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/DonCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/Interface/InteractInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/DonPlayerController.h"
#include "Player/DonPlayerState.h"
#include "UI/HUD/DonHUD.h"

ADonCharacter::ADonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

void ADonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void ADonCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void ADonCharacter::InitAbilityActorInfo()
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	check(DonPlayerState);
	DonPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DonPlayerState, this);

	Cast<UDonAbilitySystemComponent>(DonPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	AbilitySystemComponent = DonPlayerState->GetAbilitySystemComponent();
	AttributeSet = DonPlayerState->GetAttributeSet();

	if (ADonPlayerController* DonPlayerController = Cast<ADonPlayerController>(GetController()))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(DonPlayerController->GetHUD()))
		{
			DonHUD->InitOverlay(DonPlayerController, DonPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}

bool ADonCharacter::ExecuteInteract(AActor* Actor)
{
	if (Actor->Implements<UInteractInterface>())
	{
		Cast<IInteractInterface>(Actor)->Interact(GetPlayerState());
		return true;
	}
	return false;
}