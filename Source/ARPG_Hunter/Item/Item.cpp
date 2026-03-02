// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "Data/ItemData.h"

#include "Interface/Effectable.h"
#include "Data/EffectData.h"

#include "Core/Subsystem/DataManager.h"
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
		_target->ApplyEffect(Effect);
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

void UEquipmentItem::GetUpgradeStat(UWorld* WorldContext, TMap<ECharacterStatType, uint16>& _outUpgradeStat)
{
	TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(GetConfig());
	_outUpgradeStat = EquipmentConfig->Stat;

	TObjectPtr<UDataManager> DataManager = WorldContext->GetGameInstance()->GetSubsystem<UDataManager>();
	
	for (uint8 i = 0; i < GetGrade(); ++i)
	{
		FEquipmentUpgradeData* UpgradeData = DataManager->GetUpgradeData(EquipmentConfig->Rank, i, EquipmentConfig->Type);
		for (const TPair<ECharacterStatType, uint16>& Pair : UpgradeData->StatPerStep)
		{
			uint16* Val = _outUpgradeStat.Find(Pair.Key);

			if (Val == nullptr)
			{
				_outUpgradeStat.Add(Pair.Key, Pair.Value);
				continue;
			}
			
			(*Val) += Pair.Value;
		}
	}
}
