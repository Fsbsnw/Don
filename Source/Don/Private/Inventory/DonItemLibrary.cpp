// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DonItemLibrary.h"

#include "DonGameModeBase.h"
#include "GameInstance/DonGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DonPlayerState.h"
#include "UI/HUD/DonHUD.h"
#include "UI/WidgetController/DonWidgetController.h"
#include "UI/WidgetController/QuestListWidgetController.h"

FItem UDonItemLibrary::FindItemByName(const UObject* WorldContextObject, FName ItemName)
{
	const ADonGameModeBase* DonGameMode = Cast<ADonGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (DonGameMode == nullptr) return FItem();

	for (FItem Item : DonGameMode->ItemInfoAsset->ItemInformation)
	{
		if (Item.ItemName == ItemName)
		{
			return Item;
		}
	}
	
	return FItem();
}

UInventoryWidgetController* UDonItemLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(PC->GetHUD()))
		{
			ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DonHUD->GetInventoryWidgetController(WidgetControllerParams);
		}
	}
	
	return nullptr;
}

UQuestListWidgetController* UDonItemLibrary::GetQuestListWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(PC->GetHUD()))
		{
			ADonPlayerState* PS = PC->GetPlayerState<ADonPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DonHUD->GetQuestListWidgetController(WidgetControllerParams);
		}
	}
	
	return nullptr;
}

bool UDonItemLibrary::FindDialogueRow(const UObject* WorldContextObject, FDialogue& OutDialogue , const ENPCName& NPCName, const EDialogueFlow& Flow, int32 Branch, int32 Progress)
{
	if (const UDonGameInstance* DonGameInstance = Cast<UDonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		TMap<ENPCName, FDialogueContainer> DialogueTable = DonGameInstance->DialogueDataTable;
		FDialogueContainer* DialoguesForNPC = DialogueTable.Find(NPCName);
		if (DialoguesForNPC == nullptr || DialoguesForNPC->Dialogues.Num() == 0) return false;
		
		for (FDialogue Dialogue : DialoguesForNPC->Dialogues)
		{
			if (Dialogue.DialogueFlow == Flow && Dialogue.DialogueBranch == Branch && Dialogue.DialogueProgress == Progress)
			{
				OutDialogue = Dialogue;
				return true;
			}
		}
	}	
	return false;
}
