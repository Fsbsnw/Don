// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/EquipmentComponent.h"

#include "GameFramework/Character.h"
#include "Inventory/DonItemLibrary.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UEquipmentComponent::SpawnAndAttachEquipment(const FItem& Item)
{
	// 장비가 이미 장착중인 경우
	if (EquipmentParts.Contains(Item.ItemTag))
	{
		ADonEquipmentActor* Existing = EquipmentParts[Item.ItemTag];
		if (Existing)
		{
			const FItem& ItemInstance = Existing->GetEquipmentInfo();
			DetachAndDestroyEquipment(ItemInstance);
			// 같은 장비인 경우
			if (Item.IsSameInstance(ItemInstance))
			{
				return;
			}
		}
	}
	
	FItemEquipmentInfo EquipmentInfo = UDonItemLibrary::FindItemEquipmentByName(this, Item.ItemName);
	if (UClass* LoadedClass = EquipmentInfo.ItemActorClass.LoadSynchronous())
	{
		if (ADonEquipmentActor* Equipment = GetWorld()->SpawnActor<ADonEquipmentActor>(LoadedClass))
		{
			// 장비 Attribute 초기화
			Equipment->InitEquipmentAttributes();
			Equipment->SetEquipmentInfo(Item);

			EquipmentParts.Add(Item.ItemTag, Equipment);

			// 장비 메시 부착
			Equipment->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, EquipmentSockets[Item.ItemTag]);

			OnEquipmentChanged.Broadcast();
		}
	}
}

void UEquipmentComponent::DetachAndDestroyEquipment(const FItem& Item)
{
	if (EquipmentParts.Contains(Item.ItemTag))
	{
		ADonEquipmentActor* Existing = EquipmentParts[Item.ItemTag];
		if (Existing && Item.IsSameInstance(Existing->GetEquipmentInfo()))
		{
			Existing->Destroy();
			EquipmentParts.Remove(Item.ItemTag);
			OnEquipmentChanged.Broadcast();
		}
	}
}