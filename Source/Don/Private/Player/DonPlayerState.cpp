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

void ADonPlayerState::BeginPlay()
{
	Super::BeginPlay();

	InventoryComponent->InitAndLoadInventory();
}

void ADonPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADonPlayerState, Level);
	DOREPLIFETIME(ADonPlayerState, XP);
	DOREPLIFETIME(ADonPlayerState, AttributePoints);
	DOREPLIFETIME(ADonPlayerState, SkillPoints);
	DOREPLIFETIME(ADonPlayerState, Money);
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
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ADonPlayerState::AddToSkillPoints(int32 InPoints)
{
	SkillPoints += InPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void ADonPlayerState::AddToMoney(int32 InMoney)
{
	Money += InMoney;
	OnMoneyChangedDelegate.Broadcast(Money);
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

void ADonPlayerState::SetMoney(int32 InMoney)
{
	SkillPoints = InMoney;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void ADonPlayerState::SetQuestState(FQuest Quest, EQuestState State)
{
	TArray<FQuest>& QuestList = PlayerQuests.Find(Quest.QuestNPC)->Quests;
	int32 Index = QuestList.Find(Quest);
	if (Index == INDEX_NONE) return;
	
	QuestList[Index].QuestState = State;
	if (State == EQuestState::Completed && Quest.QuestType == EQuestType::MainQuest)
	{
		const UDataTable* DialogueTable = Quest.DialogueAfterQuestCompletion.DataTable;
		const FName DialogueName = Quest.DialogueAfterQuestCompletion.RowName;
		const FDialogue* Dialogue = DialogueTable->FindRow<FDialogue>(DialogueName, "");
		CompletedDialogues.Find(Quest.QuestNPC)->Dialogues.AddUnique(*Dialogue);
	}
	
	UE_LOG(LogTemp, Warning,
		TEXT("NPC : %s, Title : %s, State : %s"),
		*UEnum::GetValueAsString(QuestList[Index].QuestNPC),
		*QuestList[Index].QuestTitle,
		*UEnum::GetValueAsString(QuestList[Index].QuestState)
	);
}

void ADonPlayerState::CheckQuestObjectives()
{
	for (TTuple<ENPCName, FQuestContainer> QuestsForNPC : PlayerQuests)
	{
		for (FQuest Quest : QuestsForNPC.Value.Quests)
		{
			if (Quest.QuestState == EQuestState::Completed) continue;
			
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
				default:
					UE_LOG(LogTemp, Warning, TEXT("Unhandled ObjectiveType!"));
						break;
				}
			}

			// Do Next Function
			if (bAllObjectivesMet)
			{
				UE_LOG(LogTemp, Warning, TEXT("All Objectives Met of Quest : %s"), *Quest.QuestTitle);
				OnQuestObjectivesMet.Broadcast(Quest);
			}
		}
	}
}

bool ADonPlayerState::HasQuest(FQuest Quest)
{
	if (PlayerQuests.Find(Quest.QuestNPC) == nullptr) return false;
	
	for (FQuest PlayerQuest : PlayerQuests.Find(Quest.QuestNPC)->Quests)
	{
		if (PlayerQuest == Quest) return true;
	}
	return false;
}


// Check Item
bool ADonPlayerState::IsItemConditionMet(const FObjective& Objective)
{
	// Check Inventory
	FItem ItemToFind = UDonItemLibrary::FindItemByName(GetWorld(), Objective.ItemID);
	int32 Index = GetInventoryComponent()->FindItemInInventory(ItemToFind);
	
	return Index != INDEX_NONE && GetInventoryComponent()->GetInventory()[Index].Amount >= Objective.ItemAmount;
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
		
		return DialogueContainer && DialogueContainer->Dialogues.Contains(*Dialogue);
	}
	return false;
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
			const int32 Index = QuestContainer->Quests.Find(*Quest);
			return Index != INDEX_NONE && QuestContainer->Quests[Index].QuestState == EQuestState::Completed;
		}		
	}
	return false;
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

void ADonPlayerState::OnRep_Money(int32 OldMoney)
{
	OnMoneyChangedDelegate.Broadcast(Money);
}
