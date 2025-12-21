// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CuisineAsset.h"
#include "GameFramework/Character.h"
#include "InnCustomer.generated.h"

struct FPathFollowingResult;
class UBehaviorTree;
class AInnSeat;
struct FKitchenOrder;

UENUM(BlueprintType)
enum class ECustomerInnState : uint8
{
	Kitchen      UMETA(DisplayName = "Kitchen"),  
	Room         UMETA(DisplayName = "Room"),
	Exit         UMETA(DisplayName = "Exit"),           
};

UENUM(BlueprintType)
enum class ECustomerMealState : uint8
{
	WaitingForFood      UMETA(DisplayName = "Waiting For Food"),  
	Eating              UMETA(DisplayName = "Eating"),           
	FinishedEating      UMETA(DisplayName = "Finished Eating")   
};

UENUM(BlueprintType)
enum class ECustomerSeatState : uint8
{
	Idle				UMETA(DisplayName = "Idle"),  
	MoveToSeat          UMETA(DisplayName = "Move To Seat"),           
	Sit					UMETA(DisplayName = "Sit")   
};

UCLASS()
class DON_API AInnCustomer : public ACharacter
{
	GENERATED_BODY()

public:
	AInnCustomer();

	// Kitchen

	FKitchenOrder FoodOrder;
	
	UFUNCTION(BlueprintCallable)
	void OrderFood();
	
	UFUNCTION(BlueprintCallable)
	void ReceivedFood();
	
	void FinishMeal();
	
	UFUNCTION(BlueprintCallable)
	void EnterRoom();

	FVector SetDestination(const ECustomerInnState Destination);

	int32 GetChefLevel() const;
	int32 GetFoodPrice() const { return FoodOrder.Price; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECustomerMealState GetMealState() const { return MealState; };
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECustomerInnState GetInnState() const { return InnState; };
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECustomerSeatState GetSeatState() const { return SeatState; };

	// Seat

	UFUNCTION(BlueprintCallable)
	void RequestSeat();
	void SitOnSeat();

	// Test

	UPROPERTY(EditAnywhere)
	bool bGoToRoom = true;
	
private:
	int32 GroupSize = 1;
	
	ECustomerInnState InnState = ECustomerInnState::Kitchen;
	ECustomerMealState MealState = ECustomerMealState::WaitingForFood;
	ECustomerSeatState SeatState = ECustomerSeatState::Idle;
	
	UPROPERTY(EditAnywhere)
	TArray<FName> FavoriteFoods;
	
	UPROPERTY(EditAnywhere)
	float EatingTime = 10.f;

	UPROPERTY(VisibleInstanceOnly)
	AInnSeat* Seat = nullptr;
};
