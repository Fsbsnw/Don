// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InnTable.generated.h"

class AInnSeat;

UCLASS()
class DON_API AInnTable : public AActor
{
	GENERATED_BODY()
	
public:	
	AInnTable();

	UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade")
	void OnTableUpgraded(bool NewState);

	UFUNCTION(BlueprintCallable)
	void RegisterSeats(int32 InTableNumber, int32 InRequiredLevel);

	void SetAvailable(bool NewState);
	
	UPROPERTY(VisibleAnywhere)
	TArray<AInnSeat*> Seats;

	UPROPERTY(EditDefaultsOnly)
	int32 TableNumber = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 RequiredInteriorLevel = 1;

	UPROPERTY(EditDefaultsOnly)
	bool bIsUpgraded = false;
};
