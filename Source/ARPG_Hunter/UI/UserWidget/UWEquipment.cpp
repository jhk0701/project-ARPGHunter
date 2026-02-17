// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UserWidget/UWEquipment.h"
#include "Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

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

	if (StatInfoUIClass) 
	{
		for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
		{
			ECharacterStatType Type = static_cast<ECharacterStatType>(i);
			TObjectPtr<UUWEquipmentStatInfo> Inst = Cast<UUWEquipmentStatInfo>(CreateWidget(this, StatInfoUIClass));
			Inst->SetStatName(Type);
			StatInfos.Add(Type, Inst);
			StatContainer->AddChild(Inst);
		}
	}
}

void UUWEquipment::Init(const TMap<EEquipmentType, TObjectPtr<UEquipmentItem>>& _container, const TMap<ECharacterStatType, uint32>& _equipmentStat)
{
	for (const TPair<EEquipmentType, TObjectPtr<UEquipmentItem>>& Pair : _container)
		Slots[Pair.Key]->SetItem(Pair.Value);

	for (const TPair<ECharacterStatType, uint32>& Pair : _equipmentStat)
		StatInfos[Pair.Key]->SetStatValue(Pair.Value);
}

void UUWEquipment::SetSlot(EEquipmentType _type, TObjectPtr<UEquipmentItem> _item)
{
	Slots[_type]->SetItem(_item);
}

void UUWEquipment::SetStatInfo(const TMap<ECharacterStatType, uint32>& _mapStat)
{
	for (const TPair<ECharacterStatType, uint32>& Pair : _mapStat)
		StatInfos[Pair.Key]->SetStatValue(Pair.Value);
}

void UUWEquipmentStatInfo::SetStatName(ECharacterStatType _type)
{
	NameLabel->SetText(FText::FromName(EnumToName(_type)));
}

void UUWEquipmentStatInfo::SetStatValue(uint32 _value)
{
	ValueLabel->SetText(FText::AsNumber(_value));
}
