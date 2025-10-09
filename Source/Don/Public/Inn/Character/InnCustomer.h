// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InnCustomer.generated.h"

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
	
	int32 GroupSize = 1;

	UPROPERTY(BlueprintReadOnly)
	ECustomerMealState MealState = ECustomerMealState::WaitingForFood;
	
	UPROPERTY(BlueprintReadOnly)
	ECustomerInnState InnState = ECustomerInnState::Kitchen;
	
	UPROPERTY(EditDefaultsOnly)
	AActor* InnEntrance = nullptr;
	UPROPERTY(EditDefaultsOnly)
	AActor* Seat = nullptr;
	UPROPERTY(EditDefaultsOnly)
	AActor* RoomEntrance = nullptr;
	UPROPERTY(EditDefaultsOnly)
	AActor* Exit = nullptr;
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FName> FavoriteFoods;
	UPROPERTY(EditAnywhere)
	float EatingTime = 10.f;

};
