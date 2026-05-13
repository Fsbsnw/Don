// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Data/Quest.h"
#include "Data/Dialogue.h"
#include "GameFramework/PlayerState.h"
#include "Interface/SaveableInterface.h"
#include "DonPlayerState.generated.h"

class UInnStoreComponent;
class UInnManagerComponent;
class UNiagaraSystem;
class ULevelUpInfo;
class UInventoryComponent;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestListChanged, FQuest, Quest);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestObjectivesMet, FQuest /*StatValue*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*StatValue*/, bool /*bLevelUp*/)

/**
 * 
 */
UCLASS()
class DON_API ADonPlayerState : public APlayerState, public IAbilitySystemInterface, public ISaveableInterface
{
	GENERATED_BODY()
public:
	ADonPlayerState();

protected:
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInnManagerComponent> InnManagerComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInnStoreComponent> InnStoreComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInventoryComponent> InventoryComponent;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	// Saveable Interface

	virtual void PostInitializeComponents() override;
	virtual void SavePlayerData(FPlayerSaveData& Data) override;
	virtual void LoadPlayerData(const FPlayerSaveData& InData) override;

	// Saveable Interface

	UFUNCTION(BlueprintCallable, Category = "Inn")
	UInnManagerComponent* GetInnManagerComponent() const { return InnManagerComponent; }
	UFUNCTION(BlueprintCallable, Category = "Inn")
	UInnStoreComponent* GetInnStoreComponent() const { return InnStoreComponent; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	USoundBase* MoneyGainSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	USoundBase* XPGainSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UNiagaraSystem* LevelUpEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	USoundBase* LevelUpSound;

	void PlayInteractionSound2D(USoundBase* SoundSource);
	void ShowNiagaraEffect(UNiagaraSystem* NiagaraSystem);

	// Delegates
	
	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnLevelChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSkillPointsChangedDelegate;
	FOnPlayerStatChanged OnMoneyChangedDelegate;
	FOnPlayerStatChanged OnMemoryFragmentChangedDelegate;
	FOnPlayerStatChanged OnGameScoreChangedDelegate;
	FOnPlayerStatChanged OnKillCountChangedDelegate;
	FOnPlayerStatChanged OnAxeUpgradeChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestListChanged OnQuestListChanged;
	FOnQuestObjectivesMet OnQuestObjectivesMet;
	
	// End Delegates
	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SkillPoints; }
	FORCEINLINE int32 GetMoney() const { return Money; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetGameScore() const { return GameScore; }
	FORCEINLINE int32 GetKillCount() const { return KillCount; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetAxeUpgrade() const { return AxeUpgrade; }

	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InPoints);
	void AddToSkillPoints(int32 InPoints);
	void AddToMoney(int32 InMoney);
	void AddToScore(int32 InScore);
	void AddToKillCount(int32 InKillCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UpgradeAxe();
	void AddToAxeUpgrade(int32 InAxeUpgrade);
	
	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);
	void SetAttributePoints(int32 InPoints);
	void SetSkillPoints(int32 InPoints);
	void SetMoney(int32 InMoney);
	void SetGameScore(int32 InGameScore);
	void SetKillCount(int32 InKillCount);

	
	UPROPERTY(BlueprintReadWrite)
	TMap<ENPCName, FDialogueContainer> CompletedDialogues;
	UPROPERTY(BlueprintReadWrite)
	TMap<ENPCName, FQuestContainer> PlayerQuests;

	UFUNCTION(BlueprintCallable)
	void AddQuest(FQuest NewQuest);
	void SetQuestState(FQuest Quest, EQuestState State);
	UFUNCTION(BlueprintCallable)
	void CheckAllQuestObjectives();
	UFUNCTION(BlueprintCallable)
	bool IsQuestObjectiveCompleted(FQuest Quest);
	bool HasQuest(FQuest Quest);
	UFUNCTION(BlueprintCallable)
	FQuest GetPlayerQuestInfo(FString QuestTitle);

	UFUNCTION(BlueprintCallable)
	bool IsItemConditionMet(FString QuestTitle, const FObjective& Objective);
	UFUNCTION(BlueprintCallable)
	bool IsDialogueConditionMet(FString QuestTitle, const FObjective& Objective);
	UFUNCTION(BlueprintCallable)
	bool IsQuestConditionMet(FString QuestTitle, const FObjective& Objective);
	UFUNCTION(BlueprintCallable)
	bool IsKillCountConditionMet(FString QuestTitle, const FObjective& Objective);

private:
	UPROPERTY(EditDefaultsOnly)
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere)
	int32 XP = 0;
	UPROPERTY(EditAnywhere)
	int32 AxeUpgrade = 0;	
	UPROPERTY(VisibleAnywhere)
	int32 AttributePoints = 10;
	UPROPERTY(VisibleAnywhere)
	int32 SkillPoints = 0;
	UPROPERTY(VisibleAnywhere)
	int32 Money = 0;
	UPROPERTY(VisibleAnywhere)
	int32 GameScore = 0;
	UPROPERTY(VisibleAnywhere)
	int32 KillCount = 0;
};

