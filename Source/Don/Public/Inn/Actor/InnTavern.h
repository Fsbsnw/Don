// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InnTavern.generated.h"

class AInnTable;

UCLASS()
class DON_API AInnTavern : public AActor
{
	GENERATED_BODY()
	
public:	
	AInnTavern();

	UFUNCTION(BlueprintCallable)
	void RegisterTables();

	UFUNCTION(BlueprintCallable)
	void UpgradeTavernInterior(int32 InteriorLevel);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AInnTable*> Tables;
};
