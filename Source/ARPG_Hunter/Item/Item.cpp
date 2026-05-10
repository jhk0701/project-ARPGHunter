// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"

#include "Interface/Effectable.h"
#include "Define/Enum.h"
#include "Core/Subsystem/DataManager/DataManager.h"
#include "Data/ItemData.h"
#include "Data/EffectData.h"
#include "Data/EquipmentUpgradeData.h"


void UItem::Init(const FName& _id, EItemType _type, uint16 _amount, TObjectPtr<UItemConfig> _config)
{
	ID = _id;
	Type = _type;
	Amount = _amount;
	Config = _config;
}

bool UItem::TryAddAmount(uint16 _amount, uint16& _outRemain)
{
	if (Amount + _amount > Config->MaxAmount)
	{
		_outRemain = Amount + _amount - Config->MaxAmount;
		Amount = Config->MaxAmount;
		return false;
	}

	Amount += _amount;
	return true;
}

bool UItem::TrySubAmount(uint16 _amount)
{
	if (Amount < _amount)
		return false;

	Amount -= _amount;
	return true;
}

bool UItem::IsFull() const
{
	return Config->MaxAmount == Amount;
}

void UItem::GetItemName(FString& _outNameStr) const
{
	_outNameStr = Config->Name;
}

void UConsumableItem::Consume(IEffectable* _target)
{
	TObjectPtr<UConsumableItemConfig> ConsumableConfig = Cast<UConsumableItemConfig>(GetConfig());
	for (const TObjectPtr<UEffectData> Effect : ConsumableConfig->Effects)
	{
		FApplyEffectParam Param;
		Param.EffectData = Effect;

		_target->ApplyEffect(Param);
	}
}

void UEquipmentItem::Init(const FName& _id, EItemType _type, uint16 _amount, TObjectPtr<UItemConfig> _config)
{
	check(_amount == 1);

	Super::Init(_id, _type, 1, _config);
}

void UEquipmentItem::GetItemName(FString& _outNameStr) const
{
	Super::GetItemName(_outNameStr);

	if (Grade > 0)
		_outNameStr.Append(FString::Printf(TEXT(" (+%d)"), Grade));
}

void UEquipmentItem::GetStat(TObjectPtr<UDataManager> _dataManager, TMap<ECharacterStatType, uint32>& _outEquipmentStat)
{
	TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(GetConfig());
	_outEquipmentStat = EquipmentConfig->Stat;

	for (uint8 i = 1; i <= GetGrade(); ++i)
	{
		FEquipmentUpgradeData* UpgradeData = _dataManager->GetUpgradeData(EquipmentConfig->Rank, i, EquipmentConfig->Type);
		ensure(UpgradeData);

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

EEquipmentType UEquipmentItem::GetEquipmentType() const
{
	return Cast<UEquipmentItemConfig>(GetConfig())->Type;
}

EWeaponType UEquipmentItem::GetWeaponType() const
{
	if (GetEquipmentType() != EEquipmentType::WEAPON)
		return EWeaponType::END;

	return Cast<UWeaponItemConfig>(GetConfig())->WeaponType;
}
