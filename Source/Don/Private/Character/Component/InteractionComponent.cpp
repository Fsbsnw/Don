// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/InteractionComponent.h"

#include "Character/Interface/InteractInterface.h"
#include "Character/Player/DonCharacter.h"
#include "Components/SphereComponent.h"
#include "Player/DonPlayerController.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::SetInteractionCollision(USphereComponent* Collision)
{
	InteractionCollision = Collision;
}

void UInteractionComponent::Interact()
{
	TArray<AActor*> OverlappingActors;
	InteractionCollision->GetOverlappingActors(OverlappingActors, TSubclassOf<AActor>());

	for (AActor* NPCActor : OverlappingActors)
	{
		if (NPCActor->Implements<UInteractInterface>())
		{
			if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner()))
			{
				// 상호작용 가능한 오브젝트는 FInteractionWidgetContext 반환
				const FInteractionWidgetContext& IWC = IInteractInterface::Execute_Interact(NPCActor);
				// 단순 아이템 획득인 경우엔 종료
				if (IWC.InteractionType == EInteractionType::PickupItem) return;

				ADonPlayerController* DPC = Cast<ADonPlayerController>(PlayerCharacter->GetController());
				if (DPC == nullptr) return;
				// 상호작용 위젯 열기
				DPC->RequestOpenInteractionWidget(IWC.WidgetTag, IWC);				
				return;
			}
		}
	}
}
