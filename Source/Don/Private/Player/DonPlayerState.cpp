// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DonPlayerState.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/DonAbilitySystemComponent.h"
#include "AbilitySystem/DonAttributeSet.h"
#include "Data/LevelUpInfo.h"
#include "Inventory/DonItemLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ADonPlayerState::ADonPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDonAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

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
	DOREPLIFETIME(ADonPlayerState, MemoryFragment);
	DOREPLIFETIME(ADonPlayerState, GameScore);
}

UAbilitySystemComponent* ADonPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADonPlayerState::PlayInteractionSound2D(USoundBase* SoundSource)
{
	if (SoundSource)
	{
		UGameplayStatics::PlaySound2D(this, SoundSource, 1);
	}
}

void ADonPlayerState::ShowNiagaraEffect(UNiagaraSystem* NiagaraSystem)
{
	if (NiagaraSystem)
	{
		ACharacter* Character = Cast<ACharacter>(GetPawn());
		
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraSystem,
			Character->GetMesh(),
			FName("root"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true,
			false
		);
		NiagaraComponent->SetWorldScale3D(FVector(2.f));
		NiagaraComponent->Activate(true);
	}
}

void ADonPlayerState::AddQuest(FQuest NewQuest)
{
	if (!PlayerQuests.FindOrAdd(NewQuest.QuestNPC).Quests.Contains(NewQuest))
	{
		OnQuestListChanged.Broadcast(NewQuest);
	}
	PlayerQuests.FindOrAdd(NewQuest.QuestNPC).Quests.AddUnique(NewQuest);
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

void ADonPlayerState::CheckAllQuestObjectives()
{
	for (TTuple<ENPCName, FQuestContainer> QuestRow : PlayerQuests)
	{
		for (FQuest Quest : QuestRow.Value.Quests)
		{
			bool bAllObjectivesMet = IsQuestObjectiveCompleted(Quest);
			if (bAllObjectivesMet)
			{
				OnQuestObjectivesMet.Broadcast(Quest);
			}
		}
	}
}

bool ADonPlayerState::IsQuestObjectiveCompleted(FQuest Quest)
{	
	bool bAllObjectivesMet = true;
	
	// Check Objectives
	for (FObjective Objective : Quest.QuestObjectives)
	{
		// Too much time to update quest progress, so we have to use async function
		switch (Objective.ObjectiveType)
		{
			case EObjectiveType::HasItem:
				if (!IsItemConditionMet(Quest.QuestTitle, Objective)) bAllObjectivesMet = false;
					break;
			case EObjectiveType::DialogueComplete:
				if (!IsDialogueConditionMet(Quest.QuestTitle, Objective)) bAllObjectivesMet = false;
					break;
			case EObjectiveType::QuestComplete:
				if (!IsQuestConditionMet(Quest.QuestTitle, Objective)) bAllObjectivesMet = false;
					break;
			case EObjectiveType::KillCount:
				if (!IsKillCountConditionMet(Quest.QuestTitle, Objective)) bAllObjectivesMet = false;
				break;
			default:
				break;
		}
	}

	return bAllObjectivesMet;
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

FQuest ADonPlayerState::GetPlayerQuestInfo(FString QuestTitle)
{
	FQuestContainer* QuestContainer = PlayerQuests.Find(ENPCName::Normal);
	if (QuestContainer)
	{
		for (FQuest Quest : QuestContainer->Quests)
		{
			if (QuestTitle == Quest.QuestTitle) return Quest;
		}
	}
	return FQuest();
}


// Check Item
bool ADonPlayerState::IsItemConditionMet(FString QuestTitle, const FObjective& Objective)
{
	// Check Inventory
	FItem ItemToFind = UDonItemLibrary::FindItemByName(GetWorld(), Objective.ItemID);
	int32 Index = GetInventoryComponent()->FindItemInInventory(ItemToFind);
	
	return Index != INDEX_NONE && GetInventoryComponent()->GetInventory()[Index].Amount >= Objective.ItemAmount;
}


// Check Dialogue
bool ADonPlayerState::IsDialogueConditionMet(FString QuestTitle, const FObjective& Objective)
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
bool ADonPlayerState::IsQuestConditionMet(FString QuestTitle, const FObjective& Objective)
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


bool ADonPlayerState::IsKillCountConditionMet(FString QuestTitle, const FObjective& Objective)
{
	return (Objective.ItemAmount <= KillCount);
}

void ADonPlayerState::AddToXP(int32 InXP)
{
	const int32 PrevLevel = LevelUpInfo->FindLevelForXP(XP); 
	
	XP += InXP;

	const int32 CurrLevel = LevelUpInfo->FindLevelForXP(XP);

	if (PrevLevel < CurrLevel)
	{
		AddToLevel(CurrLevel - PrevLevel);
		int32 Rewards = LevelUpInfo->FindAttributeRewardForLevelUp(PrevLevel, CurrLevel);
		AddToAttributePoints(Rewards);
	}
	
	OnXPChangedDelegate.Broadcast(XP);
	PlayInteractionSound2D(XPGainSound);
}

void ADonPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	if (UDonAbilitySystemComponent* DonASC = Cast<UDonAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		DonASC->UpdateAbilityStatuses(Level);
	}

	
	ShowNiagaraEffect(LevelUpEffect);
	PlayInteractionSound2D(LevelUpSound);
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
	PlayInteractionSound2D(MoneyGainSound);
}

void ADonPlayerState::AddToMemoryFragment(int32 InMemoryFragment)
{
	MemoryFragment += InMemoryFragment;
	OnMemoryFragmentChangedDelegate.Broadcast(MemoryFragment);
}

void ADonPlayerState::AddToScore(int32 InGameScore)
{
	GameScore += InGameScore;
	OnGameScoreChangedDelegate.Broadcast(GameScore);
}

void ADonPlayerState::AddToKillCount(int32 InKillCount)
{
	KillCount += InKillCount;
	OnKillCountChangedDelegate.Broadcast(KillCount);
}

bool ADonPlayerState::UpgradeAxe()
{
	FItem Item = UDonItemLibrary::FindItemByName(this, FName("Upgrade Crystal"));
	Item.Amount = 1;
	TArray<FItem> Items;
	Items.Add(Item);
	
	if (InventoryComponent->HasEnoughItems(Items))
	{
		int32 Index = InventoryComponent->FindItemInInventory(Item);
		if (Index != INDEX_NONE)
		{
			InventoryComponent->RemoveItem(Index, Item.Amount);
			AddToAxeUpgrade(1);
			return true;
		}
	}
	return false;
}

void ADonPlayerState::AddToAxeUpgrade(int32 InAxeUpgrade)
{
	AxeUpgrade += InAxeUpgrade;
	OnAxeUpgradeChangedDelegate.Broadcast(AxeUpgrade);
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

void ADonPlayerState::SetMemoryFragment(int32 InMemoryFragment)
{
	MemoryFragment = InMemoryFragment;
	OnMemoryFragmentChangedDelegate.Broadcast(MemoryFragment);
}

void ADonPlayerState::SetGameScore(int32 InGameScore)
{
	GameScore = InGameScore;
	OnGameScoreChangedDelegate.Broadcast(GameScore);
}

void ADonPlayerState::SetKillCount(int32 InKillCount)
{
	KillCount = InKillCount;
	OnKillCountChangedDelegate.Broadcast(KillCount);
}


bool ADonPlayerState::RestoreMemory(int32 MemoryCost)
{
	if (MemoryFragment < MemoryCost) return false;

	AddToMemoryFragment(-MemoryCost);
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

void ADonPlayerState::OnRep_Money(int32 OldMoney)
{
	OnMoneyChangedDelegate.Broadcast(Money);
}

void ADonPlayerState::OnRep_MemoryFragment(int32 OldMemoryFragment)
{
	OnMemoryFragmentChangedDelegate.Broadcast(MemoryFragment);
}

void ADonPlayerState::OnRep_GameScore(int32 OldGameScore)
{
	OnGameScoreChangedDelegate.Broadcast(GameScore);
}

void ADonPlayerState::OnRep_KillCount(int32 OldKillCount)
{
	OnKillCountChangedDelegate.Broadcast(KillCount);
}
