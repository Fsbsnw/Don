// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn/Actor/InnSeat.h"

#include "Components/ArrowComponent.h"

AInnSeat::AInnSeat()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	RootComponent = SceneRoot;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	Mesh->SetupAttachment(RootComponent);

	SitPoint = CreateDefaultSubobject<UStaticMeshComponent>("Sit Point");
	SitPoint->SetupAttachment(Mesh);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow Component");
	ArrowComponent->SetupAttachment(Mesh);
}