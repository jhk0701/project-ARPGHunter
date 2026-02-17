// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UserWidget/UWEquipment.h"
#include "Components/UniformGridPanel.h"

#include "Define/Enum.h"
#include "Item/Item.h"
#include "UI/UserWidget/UWItemSlot.h"

void UUWEquipment::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const uint8 TYPE_COUNT = static_cast<uint8>(EEquipmentType::END);
	ensure(TYPE_COUNT == SlotContainer->GetChildrenCount());

	// UI 상에서 갯수만큼 배치해둘 것
	for (uint8 i = 0; i < TYPE_COUNT; ++i)
	{
		TObjectPtr<UUWItemSlot> ItemSlotUI = Cast<UUWItemSlot>(SlotContainer->GetChildAt(i));
		ItemSlotUI->ClearSlot();
		Slots.Add(static_cast<EEquipmentType>(i), ItemSlotUI);
	}
}

void UUWEquipment::Init(const TMap<EEquipmentType, TObjectPtr<UEquipmentItem>>& _container)
{
	for (const TPair<EEquipmentType, TObjectPtr<UEquipmentItem>>& Pair : _container)
		Slots[Pair.Key]->SetItem(Pair.Value);
}

void UUWEquipment::SetSlot(EEquipmentType _type, TObjectPtr<UEquipmentItem> _item)
{
	Slots[_type]->SetItem(_item);
}
