// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InnSeat.generated.h"

class UArrowComponent;

UCLASS()
class DON_API AInnSeat : public AActor
{
	GENERATED_BODY()
	
public:	
	AInnSeat();

protected:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneRoot;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* ArrowComponent;

public:
	FORCEINLINE bool GetIsOccupied() const { return bIsOccupied; }
	FORCEINLINE void SetIsOccupied(const bool& bNewOccupied) { bIsOccupied = bNewOccupied; }

private:
	bool bIsOccupied = false;
};
