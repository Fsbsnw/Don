// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/InteractComponent.h"

#include "UI/Widget/DonUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Character/NPC/NPCCharacterBase.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"
#include "UI/WidgetController/InteractWidgetController.h"
#include "UI/WidgetController/StoreWidgetController.h"

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

UStoreWidgetController* UInteractComponent::GetStoreWidgetController(AActor* Owner, APlayerState* TargetPlayerState)
{
	if (StoreWidgetController == nullptr)
	{
		StoreWidgetController = NewObject<UStoreWidgetController>(this, StoreWidgetControllerClass);
		StoreWidgetController->SetWidgetControllerParams(Cast<ANPCCharacterBase>(Owner)->GetInteractComponent(), TargetPlayerState);
		StoreWidgetController->BindCallbacksToDependencies();
	}
	return StoreWidgetController;
}

void UInteractComponent::BindCallbacksToDependencies()
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (DonPlayerState)
	{
		DonPlayerState->OnQuestObjectivesMet.AddUObject(this, &UInteractComponent::BroadcastQuestUpdate);
	}
}

void UInteractComponent::OpenStore(APlayerState* PlayerState)
{
	if (StoreWidget != nullptr) return;
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), StoreWidgetClass);
	StoreWidget = Cast<UDonUserWidget>(Widget);
	
	UStoreWidgetController* WidgetController = GetStoreWidgetController(GetOwner(), PlayerState);
	StoreWidget->SetWidgetController(WidgetController);

	StoreWidget->AddToViewport();
}

void UInteractComponent::OpenDialogue(APlayerState* PlayerState)
{
	if (DialogueWidget != nullptr) return;
	
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

void UInteractComponent::BroadcastQuestUpdate(FQuest Quest)
{
	ANPCCharacterBase* NPC = Cast<ANPCCharacterBase>(GetOwner());
	if (NPC && NPC->NPCName == Quest.QuestNPC)
	{
		NPC->NotifyQuestCompletionOnScreen(Quest);
	}
}

void UInteractComponent::ResetWidgets()
{
	StoreWidget = nullptr;
	DialogueWidget = nullptr;
}