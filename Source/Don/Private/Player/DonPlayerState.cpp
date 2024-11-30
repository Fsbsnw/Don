// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DonPlayerState.h"

#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"
#include "Data/LevelUpInfo.h"
#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Net/UnrealNetwork.h"

ADonPlayerState::ADonPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDonAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UDonAttributeSet>("AttributeSet");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory Component");
	NetUpdateFrequency = 100.f;
}

void ADonPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADonPlayerState, Level);
	DOREPLIFETIME(ADonPlayerState, XP);
	DOREPLIFETIME(ADonPlayerState, AttributePoints);
	DOREPLIFETIME(ADonPlayerState, SkillPoints);
}

UAbilitySystemComponent* ADonPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADonPlayerState::AddToXP(int32 InXP)
{
	const int32 PrevLevel = LevelUpInfo->FindLevelForXP(XP); 
	
	XP += InXP;

	const int32 CurrLevel = LevelUpInfo->FindLevelForXP(XP);

	if (PrevLevel < CurrLevel) AddToLevel(CurrLevel - PrevLevel);
	
	OnXPChangedDelegate.Broadcast(XP);
}

void ADonPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void ADonPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints = InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ADonPlayerState::AddToSkillPoints(int32 InPoints)
{
	SkillPoints = InPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void ADonPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ADonPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

void ADonPlayerState::SetAttributePoints(int32 InPoints)
{
	AttributePoints = InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ADonPlayerState::SetSkillPoints(int32 InPoints)
{
	SkillPoints = InPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void ADonPlayerState::CheckQuestObjectives()
{
	for (TTuple<ENPCName, FQuestContainer> QuestsForNPC : PlayerQuests)
	{
		for (FQuest Quest : QuestsForNPC.Value.Quests)
		{
			bool bAllObjectivesMet = true;
			
			// Check Objectives
			for (FObjective Objective : Quest.QuestObjectives)
			{
				// Too much time to update quest progress, so we have to use async function
				switch (Objective.ObjectiveType)
				{
				case EObjectiveType::HasItem:
					if (!IsItemConditionMet(Objective)) bAllObjectivesMet = false;
						break;
				case EObjectiveType::DialogueComplete:
					if (!IsDialogueConditionMet(Objective)) bAllObjectivesMet = false;
						break;
				case EObjectiveType::QuestComplete:
					if (!IsQuestConditionMet(Objective)) bAllObjectivesMet = false;
						break;
				}
			}

			// Do Next Function
			if (bAllObjectivesMet)
			{
				UE_LOG(LogTemp, Warning, TEXT("All Objectives Met"));
			}
		}
	}
}


// Check Item
bool ADonPlayerState::IsItemConditionMet(const FObjective& Objective)
{
	// Check Inventory
	FItem ItemToFind = UDonItemLibrary::FindItemByName(GetWorld(), Objective.ItemID);
	int32 ItemIndex = Inventory.Find(ItemToFind);
	if (!Inventory.Contains(ItemToFind) ||
		ItemIndex == INDEX_NONE ||
		Inventory[ItemIndex].Amount != Objective.ItemAmount)
	{
		return false;
	}
	return true;
}


// Check Dialogue
bool ADonPlayerState::IsDialogueConditionMet(const FObjective& Objective)
{
	if (Objective.ObjectiveDataHandle.DataTable)
	{
		const UDataTable* DialogueTable = Objective.ObjectiveDataHandle.DataTable;
		const FName DialogueName = Objective.ObjectiveDataHandle.RowName;

		const FDialogue* Dialogue = DialogueTable->FindRow<FDialogue>(DialogueName, TEXT(""));
		const FDialogueContainer* DialogueContainer = CompletedDialogues.Find(Dialogue->NPCName); 
		if (DialogueContainer)
		{
			if (!DialogueContainer->Dialogues.Find(*Dialogue))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}


// Check Quest
bool ADonPlayerState::IsQuestConditionMet(const FObjective& Objective)
{
	if (Objective.ObjectiveDataHandle.DataTable)
	{
		const UDataTable* QuestTable = Objective.ObjectiveDataHandle.DataTable;
		const FName QuestName = Objective.ObjectiveDataHandle.RowName;

		const FQuest* Quest = QuestTable->FindRow<FQuest>(QuestName, TEXT(""));
		const FQuestContainer* QuestContainer = PlayerQuests.Find(Quest->QuestNPC);
		if (QuestContainer)
		{
			if (!QuestContainer->Quests.Find(*Quest))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

void ADonPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void ADonPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void ADonPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ADonPlayerState::OnRep_SkillPoints(int32 OldSkillPoints)
{
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}
