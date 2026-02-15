// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "Data/ItemData.h"

void UItem::Init(uint16 _amount, TObjectPtr<UItemConfig> _config)
{
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
