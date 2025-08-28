// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InnCustomer.generated.h"

struct FKitchenOrder;

UENUM(BlueprintType)
enum class ECustomerMealState : uint8
{
	WaitingForFood      UMETA(DisplayName = "Waiting For Food"),  
	Eating              UMETA(DisplayName = "Eating"),           
	FinishedEating      UMETA(DisplayName = "Finished Eating")   
};

UCLASS()
class DON_API AInnCustomer : public ACharacter
{
	GENERATED_BODY()

public:
	AInnCustomer();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void OrderFood();
	UFUNCTION(BlueprintCallable)
	void ReceivedFood();
	void OnMealFinished();
	UFUNCTION(BlueprintCallable)
	void EnterRoom();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECustomerMealState GetMealState() const { return MealState; };

private:
	UPROPERTY(EditAnywhere)
	TArray<FKitchenOrder> FavoriteFoods;

	ECustomerMealState MealState = ECustomerMealState::WaitingForFood;
};
