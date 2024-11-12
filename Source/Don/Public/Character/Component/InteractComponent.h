// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "InteractComponent.generated.h"

USTRUCT(BlueprintType)
struct FResponseOption
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Response;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NextProgress = 1;
};

UENUM(BlueprintType)
enum class EConditionType : uint8
{
	QuestComplete     UMETA(DisplayName = "Quest Complete"),
	HasItem           UMETA(DisplayName = "Has Item"),
	ChoiceSatisfied    UMETA(DisplayName = "Choice Satisfied") 
};

USTRUCT(BlueprintType)
struct FDialogueCondition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EConditionType ConditionType; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName TargetID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString DialogueID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DialogueProgress; 
};

USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase
{
	GENERATED_BODY()

	bool operator==(const FDialogue& Other) const
	{
		return DialogueID == Other.DialogueID && DialogueProgress == Other.DialogueProgress;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString DialogueID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DialogueProgress = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DialogueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FResponseOption> ResponseOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDialogueCondition> Conditions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NextDialogueProgress;
};

USTRUCT(BlueprintType)
struct FDialogueContainer
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FDialogue> Dialogues;
};

class UInteractWidgetController;
class UDonUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractComponent();

	UInteractWidgetController* GetInteractWidgetController(AActor* Owner, APlayerState* TargetPlayerState);
	
	void OpenStore(APlayerState* PlayerState);
	void OpenDialogue(APlayerState* PlayerState);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TObjectPtr<UDonUserWidget> StoreWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<UDonUserWidget> StoreWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TObjectPtr<UDonUserWidget> DialogueWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<UDonUserWidget> DialogueWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TObjectPtr<UInteractWidgetController> InteractWidgetController;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<UInteractWidgetController> InteractWidgetControllerClass;
};
