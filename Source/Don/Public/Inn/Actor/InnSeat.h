// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InnSeat.generated.h"

UCLASS()
class DON_API AInnSeat : public AActor
{
	GENERATED_BODY()
	
public:	
	AInnSeat();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool GetIsOccupied() const { return bIsOccupied; }
	FORCEINLINE void SetIsOccupied(const bool& bNewOccupied) { bIsOccupied = bNewOccupied; }

private:
	bool bIsOccupied = false;
};
