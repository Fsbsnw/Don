// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/QuestListWidgetController.h"

#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Player/DonPlayerState.h"

void UQuestListWidgetController::BindCallbacksToDependencies()
{
	ADonPlayerState* DonPlayerState = CastChecked<ADonPlayerState>(PlayerState);
	
	DonPlayerState->OnKillCountChangedDelegate.AddLambda(
		[this](int32 NewKillCount)
		{
			OnKillCountDelegate.Broadcast(NewKillCount);
		}
	);
}

void UQuestListWidgetController::AwardQuestRewards(FString QuestTitle)
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState == nullptr) return;
	
	FQuest Quest;
	Quest = DonPlayerState->GetPlayerQuestInfo(QuestTitle);
	if (Quest.QuestTitle.IsEmpty()) return;

	// Find Items to Remove by Quest Objectives
	for (FObjective Objective : Quest.QuestObjectives)
	{
		if (Objective.ObjectiveType == EObjectiveType::HasItem)
		{
			FItem ItemToRemove = UDonItemLibrary::FindItemByName(GetWorld(), Objective.ItemID);
			int32 IndexToRemove = DonPlayerState->GetInventoryComponent()->FindItemInInventory(ItemToRemove);
			DonPlayerState->GetInventoryComponent()->RemoveItem(ItemToRemove, IndexToRemove, Objective.ItemAmount);						
		}
	}

	// Award Rewards
	for (FQuestReward Reward : Quest.QuestRewards)
	{
		// Items
		if (Reward.RewardType == EQuestRewardType::Item)
		{
			FItem ItemToAdd = UDonItemLibrary::FindItemByName(GetWorld(), Reward.ItemID);
			DonPlayerState->GetInventoryComponent()->AddItem(ItemToAdd, Reward.Amount);
		}
		// XP
		else if (Reward.RewardType == EQuestRewardType::XP)
		{
			DonPlayerState->AddToXP(Reward.Amount);
		}
		// Money
		else if (Reward.RewardType == EQuestRewardType::Money)
		{
			DonPlayerState->AddToMoney(Reward.Amount);
		}
	}

	// Set New Repeat Quest
	FQuest NextRepeatQuest;
	NextRepeatQuest.QuestTitle = Quest.QuestTitle;
	NextRepeatQuest.QuestDescription = Quest.QuestDescription;
	for (FObjective Objective : Quest.QuestObjectives)
	{
		if (Objective.ObjectiveType == EObjectiveType::KillCount)
		{
			FObjective NewObjective;
			NewObjective.ItemID = Objective.ItemID;
			NewObjective.ObjectiveType = Objective.ObjectiveType;
			NewObjective.ItemAmount = Objective.ItemAmount + 3;
			NextRepeatQuest.QuestObjectives.Add(NewObjective);
		}
	}

	NextRepeatQuest.QuestRewards = Quest.QuestRewards;
	for (FQuestReward& Reward : NextRepeatQuest.QuestRewards)
	{
		if (Reward.RewardType == EQuestRewardType::XP)
		{
			Reward.Amount += 100;				
		}
		else if (Reward.RewardType == EQuestRewardType::Item)
		{
			Reward.Amount += 2;
		}
		else if (Reward.RewardType == EQuestRewardType::Money)
		{
			Reward.Amount += 2000;
		}
	}

	// Remove Completed Quest and Add New Repeat Quest
	DonPlayerState->PlayerQuests.FindOrAdd(ENPCName::Normal).Quests.Remove(Quest);
	DonPlayerState->PlayerQuests.FindOrAdd(ENPCName::Normal).Quests.Add(NextRepeatQuest);
}

FQuest UQuestListWidgetController::FindPlayerQuest(FString QuestTitle)
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState == nullptr) return FQuest();

	return DonPlayerState->GetPlayerQuestInfo(QuestTitle);
}

int32 UQuestListWidgetController::GetPlayerKillCount()
{
	ADonPlayerState* DonPlayerState = Cast<ADonPlayerState>(PlayerState);
	if (DonPlayerState == nullptr) return 0;

	return DonPlayerState->GetKillCount();
}
