// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InnSeat.generated.h"

class AInnSeat;
class UArrowComponent;

USTRUCT(BlueprintType)
struct FSeatGroup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seat Group")
	int32 AssignedGroupID = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seat Group")
	int32 GroupSize = 0;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seat Group")
	TArray<AInnSeat*> Seats;
};

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
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SitPoint;
	
	FORCEINLINE bool GetIsOccupied() const { return bIsOccupied; }
	FORCEINLINE void SetIsOccupied(const bool& bNewOccupied) { bIsOccupied = bNewOccupied; }

	UPROPERTY(EditAnywhere)
	int32 SeatGroup = 0;

	bool bIsAvailable = false;
private:
	bool bIsOccupied = false;
};
