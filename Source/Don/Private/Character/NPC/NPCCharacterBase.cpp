// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/NPCCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

ANPCCharacterBase::ANPCCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	QuestNotification = CreateDefaultSubobject<UWidgetComponent>("Quest Notification");
	QuestNotification->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

FInteractionWidgetContext ANPCCharacterBase::Interact_Implementation()
{
	return FInteractionWidgetContext();
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