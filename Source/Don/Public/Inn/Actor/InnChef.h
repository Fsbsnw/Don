// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CuisineAsset.h"
#include "GameFramework/Actor.h"
#include "InnChef.generated.h"

USTRUCT(BlueprintType)
struct FChefUIData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* ChefImage;

	UPROPERTY(BlueprintReadOnly)
	bool bIsHired = false;
	
	UPROPERTY(BlueprintReadOnly)
	int32 ChefLevel = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 ChefXP = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChefAttributeChangedUI, const FChefUIData&, ChefUIData);

struct FKitchenOrder;

UCLASS()
class DON_API AInnChef : public AActor
{
	GENERATED_BODY()
	
public:	
	AInnChef();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* Mesh;

	FChefUIData GetChefUIData() const;
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnChefHiredChanged(bool bIsChefHired);

	void Hired(bool NewState);
	
	FORCEINLINE bool IsCooking() const { return bIsCooking; }
	void StartOrder(FKitchenOrder& Order);
	void EndOrder();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* ChefImage;

	UFUNCTION(BlueprintCallable)
	int32 GetChefLevel() const { return ChefLevel; }
	
	UFUNCTION(BlueprintCallable)
	void ChefLevelUp();

	UPROPERTY(EditDefaultsOnly)
	int32 ChefLevel = 1;
	int32 ChefXP = 0;
	
	void AddToXP(int32 InXP);

	UPROPERTY(BlueprintReadWrite)
	bool bIsHired = false;
	
	bool bIsCooking = false;
	FGuid OrderID;

	UPROPERTY(BlueprintAssignable)
	FOnChefAttributeChangedUI OnChefAttributeChanged;

	UFUNCTION(BlueprintCallable)
	void BroadcastInitialAttributes();
};