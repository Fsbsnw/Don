// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/DonCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
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
	InitAndLoadInventory();
}

void ADonCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
	InitAndLoadInventory();
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

void ADonCharacter::InitAndLoadInventory()
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	check(DonPlayerState);

	uint8 MaxSlotSize = DonPlayerState->MaxItemSlots;
	DonPlayerState->GetInventory().SetNum(MaxSlotSize);
	for (uint8 i = 0; i < MaxSlotSize; i++)
	{
		DonPlayerState->GetInventory()[i].InventorySlotIndex = i;
	}

	// Load Player State Inventory Info
}
