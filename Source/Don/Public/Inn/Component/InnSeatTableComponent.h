// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inn/Actor/InnSeat.h"
#include "InnSeatTableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInnSeatTableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInnSeatTableComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> TableMesh;

	UPROPERTY(EditDefaultsOnly)
	TArray<AInnSeat*> Seats;

	UPROPERTY(EditAnywhere)
	int32 RequiredInteriorLevel = 1;
};
