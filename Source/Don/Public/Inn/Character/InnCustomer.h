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
enum class ECustomerNotify : uint8
{
	EnterInn			UMETA(DisplayName = "EnterInn"),  
	FinishedEating      UMETA(DisplayName = "FinishedEating"),
	EnterRoom			UMETA(DisplayName = "EnterRoom"),
	ExitInn				UMETA(DisplayName = "ExitInn"),
};

UENUM(BlueprintType)
enum class ECustomerType : uint8
{
	First		UMETA(DisplayName = "First"),  
	Second      UMETA(DisplayName = "Second"),
	Third       UMETA(DisplayName = "Third"),           
};

UENUM(BlueprintType)
enum class ECustomerInnState : uint8
{
	Entrance	 UMETA(DisplayName = "Entrance"),
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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCustomerChanged, ECustomerNotify);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInnGroupStateChanged, int32, ECustomerInnState);

UCLASS()
class DON_API AInnCustomer : public ACharacter
{
	GENERATED_BODY()
protected:
	AInnCustomer();

	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<UTexture2D*> Portraits;
	
	void SetDestination(const ECustomerInnState& Destination);
	FVector GetDestination() const { return NextDestination; };

	void EnterInn();
	void ExitInn();

	// Kitchen
	
	FOnInnGroupStateChanged OnInnGroupStateChanged;
	FOnCustomerChanged OnCustomerChanged;

	void OnSeatAssigned(bool State);
	
	UFUNCTION(BlueprintCallable)
	FKitchenOrder CreateFoodOrder();

	void OrderFood();
	void ReceiveFood();
	
	void FinishMeal();
	void OnGroupMealFinished(bool State);
	void OnGroupDecidedToStay(bool State);

	int32 GetFoodPrice() const;
	FGuid GetID() const { return ID; }
	int32 GetLevel() const { return Level; }
	int32 GetGroupID() const { return GroupID; };
	int32 GetSatisfaction() const { return Satisfaction; }
	void SetGroupID(int32 NewID) { GroupID = NewID; }
	

	// Room
	
	UFUNCTION(BlueprintCallable)
	void EnterRoom();

	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECustomerType GetType() const { return CustomerType; };
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECustomerMealState GetMealState() const { return MealState; };
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECustomerInnState GetInnState() const { return InnState; };
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECustomerSeatState GetSeatState() const { return SeatState; };

	void SetInnState(ECustomerInnState State) { InnState = State; }

	// Seat

	void ReserveSeat(AInnSeat* NewSeat);
	void SitOnSeat();

	UPROPERTY(VisibleInstanceOnly)
	AInnSeat* Seat = nullptr;
	
private:
	UPROPERTY(VisibleAnywhere)
	FGuid ID;
	int32 GroupID = 0;
	int32 Level = 1;
	int32 SelectedFood = 0;
	int32 Satisfaction = 0;

	ECustomerType CustomerType = ECustomerType::First;
	ECustomerInnState InnState = ECustomerInnState::Entrance;
	ECustomerMealState MealState = ECustomerMealState::WaitingForFood;
	ECustomerSeatState SeatState = ECustomerSeatState::Idle;

	FVector NextDestination = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere)
	TArray<FName> FavoriteFoods;
	
	UPROPERTY(EditAnywhere)
	float EatingTime = 10.f;
};