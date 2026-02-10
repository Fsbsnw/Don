// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InnManagerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInnChanged, int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DON_API UInnManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInnManagerComponent();

protected:

public:
	FORCEINLINE int32 GetReputation() const { return Reputation; }
	FORCEINLINE int32 GetPopularity() const { return Popularity; }
	FORCEINLINE int32 GetInterior() const { return Interior; }

	void AddToReputation(int32 InReputation);
	void AddToPopularity(int32 InPopularity);
	void AddToInterior(int32 InInterior);

	FOnInnChanged OnReputationChanged;
	FOnInnChanged OnPopularityChanged;
	FOnInnChanged OnInteriorChanged;
	
private:
	int32 Reputation = 0;
	int32 Popularity = 0;
	int32 Interior = 0;		
};
