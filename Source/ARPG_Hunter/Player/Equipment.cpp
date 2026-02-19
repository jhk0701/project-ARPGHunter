// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Equipment.h"

#include "Define/Enum.h"
#include "Data/ItemData.h"
#include "Item/Item.h"

UEquipment::UEquipment()
{
	for (uint8 i = 0; i < static_cast<uint8>(EEquipmentType::END); ++i)
		Container.Add(static_cast<EEquipmentType>(i));

	for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
		EquipmentStat.Add(static_cast<ECharacterStatType>(i));
}

void UEquipment::Init()
{
	// TODO: 저장 데이터 반영
}

TObjectPtr<UEquipmentItem> UEquipment::Equip(EEquipmentType _type, TObjectPtr<UItem> _equipment)
{
	TObjectPtr<UEquipmentItem> NewEquipment = Cast<UEquipmentItem>(_equipment);
	check(NewEquipment);

	TObjectPtr<UEquipmentItem> PrevItem = nullptr;
	if (Container[_type] != nullptr) 
		PrevItem = Unequip(_type);

	TObjectPtr<UEquipmentItemConfig> Config = Cast<UEquipmentItemConfig>(NewEquipment->GetConfig());
	check(Config);

	for (const TPair<ECharacterStatType, uint16>& Pair : Config->Stat)
		EquipmentStat[Pair.Key] += Pair.Value;

	Container[_type] = NewEquipment;
	OnEquipmentChanged.Broadcast(_type, Container[_type]);
	OnStatValueChanged.Broadcast(EquipmentStat);

	return PrevItem;
}

TObjectPtr<UEquipmentItem> UEquipment::Unequip(EEquipmentType _type)
{
	TObjectPtr<UEquipmentItem> PrevItem = Container[_type];
	
	TObjectPtr<UEquipmentItemConfig> Config = Cast<UEquipmentItemConfig>(PrevItem->GetConfig());
	check(Config);

	for (const TPair<ECharacterStatType, uint16>& Pair : Config->Stat)
		EquipmentStat[Pair.Key] -= Pair.Value;

	Container[_type] = nullptr;
	OnEquipmentChanged.Broadcast(_type, Container[_type]);
	OnStatValueChanged.Broadcast(EquipmentStat);

	return PrevItem;
}
