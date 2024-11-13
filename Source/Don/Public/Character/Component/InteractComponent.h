// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

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
