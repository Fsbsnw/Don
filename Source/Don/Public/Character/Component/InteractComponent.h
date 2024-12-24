// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Quest.h"
#include "InteractComponent.generated.h"

class UStoreWidgetController;
class UInteractWidgetController;
class UDonUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractComponent();

	UInteractWidgetController* GetInteractWidgetController(AActor* Owner, APlayerState* TargetPlayerState);
	UStoreWidgetController* GetStoreWidgetController(AActor* Owner, APlayerState* TargetPlayerState);
	void BindCallbacksToDependencies();
	
	void OpenStore(APlayerState* PlayerState);
	void OpenDialogue(APlayerState* PlayerState);
	void BroadcastQuestUpdate(FQuest Quest);

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Interaction")
	TObjectPtr<UDonUserWidget> StoreWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<UDonUserWidget> StoreWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Interaction")
	TObjectPtr<UDonUserWidget> DialogueWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<UDonUserWidget> DialogueWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Interaction")
	TObjectPtr<UInteractWidgetController> InteractWidgetController;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<UInteractWidgetController> InteractWidgetControllerClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Interaction")
	TObjectPtr<UStoreWidgetController> StoreWidgetController;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<UStoreWidgetController> StoreWidgetControllerClass;
};
