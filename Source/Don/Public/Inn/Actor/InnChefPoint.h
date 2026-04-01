// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InnChefPoint.generated.h"

UCLASS()
class DON_API AInnChefPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AInnChefPoint();

	UPROPERTY(EditAnywhere)
	int32 PointIndex;
};
