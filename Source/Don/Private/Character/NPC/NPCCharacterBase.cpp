// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/NPCCharacterBase.h"

#include "Character/Component/InteractComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

ANPCCharacterBase::ANPCCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractComponent = CreateDefaultSubobject<UInteractComponent>("Interact Component");
	QuestNotification = CreateDefaultSubobject<UWidgetComponent>("Quest Notification");
	QuestNotification->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

UInteractComponent* ANPCCharacterBase::GetInteractComponent()
{
	return InteractComponent;
}

void ANPCCharacterBase::Interact(APlayerState* TargetPlayerState)
{
}

void ANPCCharacterBase::NotifyQuestCompletionOnScreen(FQuest Quest)
{
	if (QuestNotification == nullptr) return;

	CompletableQuests.AddUnique(Quest);
	QuestNotification->SetVisibility(true);
}

void ANPCCharacterBase::RemoveCompletedQuest(FQuest Quest)
{
	if (CompletableQuests.Contains(Quest))
	{
		CompletableQuests.Remove(Quest);
		if (CompletableQuests.Num() == 0)
		{
			QuestNotification->SetVisibility(false);
		}
		UE_LOG(LogTemp, Warning, TEXT("Removed Completed Quest!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Removed Completed Quest!"));
	}
}

void ANPCCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetInteractComponent()->BindCallbacksToDependencies();
}
