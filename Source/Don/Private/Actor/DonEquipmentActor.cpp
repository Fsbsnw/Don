// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DonEquipmentActor.h"

#include "Don.h"
#include "DonGameplayTags.h"
#include "Engine/CurveTable.h"
#include "Inventory/DonItemLibrary.h"

ADonEquipmentActor::ADonEquipmentActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADonEquipmentActor::RequestDestroy()
{
	Destroy();
}

void ADonEquipmentActor::OnBeginOverlap(AActor* TargetActor)
{
	UE_LOG(LogTemp, Warning, TEXT("%s is hit by %s"), *EquipmentName.ToString(), *TargetActor->GetName());
}

void ADonEquipmentActor::InitEquipmentAttributes()
{
	FEquipmentAttributes EquipmentAttributes = UDonItemLibrary::FindEquipmentAttributes(GetWorld(), EquipmentName);
	EquipmentName = EquipmentAttributes.EquipmentName;
	EquipmentType = EquipmentAttributes.EquipmentType;
	EquipmentBaseDamage = EquipmentAttributes.BaseDamage;
	EquipmentBaseDefense = EquipmentAttributes.BaseDefense;
	
	if (EquipmentAttributes.UpgradeCT)
	{
		FRealCurve* Curve = EquipmentAttributes.UpgradeCT->FindCurve(EquipmentName, TEXT(""));
		EquipmentUpgradedBonus = Curve->Eval(EquipmentUpgrade);
	}
}

float ADonEquipmentActor::GetFinalAttribute() const
{
	if (EquipmentType.MatchesTag(FDonGameplayTags::Get().Item_Equippable_Weapon))
	{
		return EquipmentBaseDamage + EquipmentUpgradedBonus;
	}
	if (EquipmentType.MatchesTag(FDonGameplayTags::Get().Item_Equippable_Armor))
	{
		return EquipmentBaseDefense + EquipmentUpgradedBonus;
	}
	return 0.f;
}

void ADonEquipmentActor::UpdateUpgradedPoint(int32 InPoint)
{
	EquipmentUpgrade = InPoint;

	FEquipmentAttributes EquipmentAttributes = UDonItemLibrary::FindEquipmentAttributes(GetWorld(), EquipmentName);
	
	if (EquipmentAttributes.UpgradeCT)
	{
		FRealCurve* Curve = EquipmentAttributes.UpgradeCT->FindCurve(EquipmentName, TEXT(""));
		EquipmentUpgradedBonus = Curve->Eval(EquipmentUpgrade);
	}
}
