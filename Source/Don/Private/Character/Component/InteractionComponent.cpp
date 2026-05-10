// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/InteractionComponent.h"

#include "Character/Interface/InteractInterface.h"
#include "Character/Player/DonCharacter.h"
#include "Components/SphereComponent.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InteractionCollision = CreateDefaultSubobject<USphereComponent>("Interaction Collision");
	InteractionCollision->SetupAttachment(this);
	InteractionCollision->SetSphereRadius(100.f);
	InteractionCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	InteractionCollision->SetGenerateOverlapEvents(true);
	InteractionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UInteractionComponent::Interact()
{
	TArray<AActor*> OverlappingActors;
	InteractionCollision->GetOverlappingActors(OverlappingActors, TSubclassOf<AActor>());

	for (AActor* NPCActor : OverlappingActors)
	{
		if (NPCActor->Implements<UInteractInterface>())
		{
			if (ADonCharacter* DonCharacter = Cast<ADonCharacter>(GetOwner()))
			{
				Cast<IInteractInterface>(NPCActor)->Interact(DonCharacter->GetPlayerState());				
			}			
			return;
		}
	}
}