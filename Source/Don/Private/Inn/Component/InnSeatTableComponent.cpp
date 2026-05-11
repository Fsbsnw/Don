// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Inn/Component/InnSeatTableComponent.h"

UInnSeatTableComponent::UInnSeatTableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>("Table Static Mesh");
}

void UInnSeatTableComponent::BeginPlay()
{
	Super::BeginPlay();
}

