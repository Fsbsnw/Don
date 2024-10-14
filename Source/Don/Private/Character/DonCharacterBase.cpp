// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DonCharacterBase.h"
#include "Components/CapsuleComponent.h"

ADonCharacterBase::ADonCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("hand_left"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADonCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}