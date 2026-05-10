// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Equipment/Equipment.h"

#include "Define/Enum.h"
#include "Core/Subsystem/DataManager/DataManager.h"
#include "Item/Item.h"

UEquipment::UEquipment()
{
	for (uint8 i = 0; i < static_cast<uint8>(EEquipmentType::END); ++i)
		Container.Add(static_cast<EEquipmentType>(i));

	for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
		EquipmentStat.Add(static_cast<ECharacterStatType>(i));
}

void UEquipment::Init(TWeakObjectPtr<UGameInstance> _instance)
{
	GI = _instance;
}

void UEquipment::Equip(EEquipmentType _type, TWeakObjectPtr<UItem> _equipment)
{
	TObjectPtr<UEquipmentItem> NewEquipment = Cast<UEquipmentItem>(_equipment);
	check(NewEquipment);

	if (Container[_type].Get() != nullptr) 
		Unequip(_type);

	NewEquipment->SetEquipmentIndex(static_cast<int32>(_type));

	TMap<ECharacterStatType, uint32> Stat;
	NewEquipment->GetStat(GI->GetSubsystem<UDataManager>(), Stat);

	for (const TPair<ECharacterStatType, uint32>& Pair : Stat)
		EquipmentStat[Pair.Key] += Pair.Value;

	Container[_type] = NewEquipment;
	OnEquipmentChanged.Broadcast(_type, Container[_type]);
	OnStatValueChanged.Broadcast(EquipmentStat);
}

TWeakObjectPtr<UEquipmentItem> UEquipment::Unequip(EEquipmentType _type)
{
	TWeakObjectPtr<UEquipmentItem> PrevItem = Container[_type];
	check(PrevItem.IsValid());

	PrevItem->SetEquipmentIndex(-1);

	TMap<ECharacterStatType, uint32> Stat;
	PrevItem->GetStat(GI->GetSubsystem<UDataManager>(), Stat);

	for (const TPair<ECharacterStatType, uint16>& Pair : Stat)
		EquipmentStat[Pair.Key] -= Pair.Value;

	Container[_type] = nullptr;
	OnEquipmentChanged.Broadcast(_type, Container[_type]);
	OnStatValueChanged.Broadcast(EquipmentStat);

	return PrevItem;
}

void UEquipment::LoadEquipment(TObjectPtr<UItem> _item)
{
	TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(_item);
	Equip(Equipment->GetEquipmentType(), Equipment);
}
