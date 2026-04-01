// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class DON_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	APatrolPath();

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> PatrolPoints;
};
