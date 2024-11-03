// Fill out your copyright notice in the Description page of Project Settings.


#include "DonAssetManager.h"

#include "DonGameplayTags.h"

UDonAssetManager& UDonAssetManager::Get()
{
	check(GEngine);
	
	UDonAssetManager* DonAssetManager = Cast<UDonAssetManager>(GEngine->AssetManager);
	return *DonAssetManager;
}

void UDonAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FDonGameplayTags::InitializeNativeGameplayTags();
}
