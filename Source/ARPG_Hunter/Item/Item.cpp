// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "Data/ItemData.h"


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

void UConsumableItem::Consume()
{
}


void UEquipmentItem::Init(const FName& _id, EItemType _type, uint16 _amount, TObjectPtr<UItemConfig> _config)
{
	check(_amount == 1);

	Super::Init(_id, _type, 1, _config);
}