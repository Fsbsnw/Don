// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/InteractComponent.h"

#include "UI/Widget/DonUserWidget.h"

#include "Blueprint/UserWidget.h"
#include "Character/NPC/NPCCharacterBase.h"
#include "UI/WidgetController/InteractWidgetController.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UInteractWidgetController* UInteractComponent::GetInteractWidgetController(AActor* Owner, APlayerState* TargetPlayerState)
{
	if (InteractWidgetController == nullptr)
	{
		InteractWidgetController = NewObject<UInteractWidgetController>(this, InteractWidgetControllerClass);
		InteractWidgetController->SetWidgetControllerParams(Cast<ANPCCharacterBase>(Owner)->GetInteractComponent(), TargetPlayerState);
	}
	return InteractWidgetController;
}

void UInteractComponent::OpenStore(APlayerState* PlayerState)
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), StoreWidgetClass);
	StoreWidget = Cast<UDonUserWidget>(Widget);
	
	UInteractWidgetController* WidgetController = GetInteractWidgetController(GetOwner(), PlayerState);
	StoreWidget->SetWidgetController(WidgetController);

	StoreWidget->AddToViewport();
}

void UInteractComponent::OpenDialogue(APlayerState* PlayerState)
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), DialogueWidgetClass);
	DialogueWidget = Cast<UDonUserWidget>(Widget);

	UInteractWidgetController* WidgetController = GetInteractWidgetController(GetOwner(), PlayerState);
	DialogueWidget->SetWidgetController(WidgetController);

	FVector2d ScreenSize;
	GEngine->GameViewport->GetViewportSize(ScreenSize);
	ScreenSize = FVector2d(ScreenSize.X / 5, ScreenSize.Y * 2 / 3);
	DialogueWidget->SetPositionInViewport(ScreenSize);
	
	DialogueWidget->AddToViewport();
}