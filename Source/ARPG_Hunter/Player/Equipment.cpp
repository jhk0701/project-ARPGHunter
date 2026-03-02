// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Equipment.h"

#include "Define/Enum.h"
#include "Data/ItemData.h"
#include "Item/Item.h"
#include "Data/EquipmentUpgradeData.h"

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

void UEquipment::Equip(EEquipmentType _type, TWeakObjectPtr<UItem> _equipment)
{
	ensure(IsValid());

	TObjectPtr<UEquipmentItem> NewEquipment = Cast<UEquipmentItem>(_equipment);
	check(NewEquipment);

	if (Container[_type].Get() != nullptr) 
		Unequip(_type);

	NewEquipment->SetEquipmentIndex(static_cast<int32>(_type));

	TMap<ECharacterStatType, uint32> Stat;
	GetStat(NewEquipment, Stat);

	for (const TPair<ECharacterStatType, uint32>& Pair : Stat)
		EquipmentStat[Pair.Key] += Pair.Value;

	Container[_type] = NewEquipment;
	OnEquipmentChanged.Broadcast(_type, Container[_type]);
	OnStatValueChanged.Broadcast(EquipmentStat);
}

TWeakObjectPtr<UEquipmentItem> UEquipment::Unequip(EEquipmentType _type)
{
	ensure(IsValid());

	TWeakObjectPtr<UEquipmentItem> PrevItem = Container[_type];
	check(PrevItem.IsValid());

	PrevItem->SetEquipmentIndex(-1);

	TMap<ECharacterStatType, uint32> Stat;
	GetStat(PrevItem, Stat);

	for (const TPair<ECharacterStatType, uint16>& Pair : Stat)
		EquipmentStat[Pair.Key] -= Pair.Value;

	Container[_type] = nullptr;
	OnEquipmentChanged.Broadcast(_type, Container[_type]);
	OnStatValueChanged.Broadcast(EquipmentStat);

	return PrevItem;
}

void UEquipment::GetStat(TWeakObjectPtr<UEquipmentItem> _equipment, TMap<ECharacterStatType, uint32>& _outEquipmentStat)
{
	if (IsValid() == false)
		return;

	TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(_equipment->GetConfig());
	_outEquipmentStat = EquipmentConfig->Stat;

	for (uint8 i = 1; i <= _equipment->GetGrade(); ++i)
	{
		FEquipmentUpgradeData* UpgradeData = GetUpgradeDataFunc.Execute(EquipmentConfig->Rank, i, EquipmentConfig->Type);
		for (const TPair<ECharacterStatType, uint16>& Pair : UpgradeData->StatPerStep)
		{
			uint32* Val = _outEquipmentStat.Find(Pair.Key);

			if (Val == nullptr)
			{
				_outEquipmentStat.Add(Pair.Key, Pair.Value);
				continue;
			}

			(*Val) += Pair.Value;
		}
	}
}