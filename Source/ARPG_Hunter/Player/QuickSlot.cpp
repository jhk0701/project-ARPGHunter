// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/QuickSlot.h"
#include "Item/Item.h"

UQuickSlot::UQuickSlot()
{
}

void UQuickSlot::Init(uint8 _size)
{
	Container.SetNum(_size);
}

TObjectPtr<UConsumableItem> UQuickSlot::Register(uint8 _index, TObjectPtr<UConsumableItem> _consumableItem)
{
	if (Container.Num() <= _index)
		return nullptr;

	TObjectPtr<UConsumableItem> Prev = nullptr;
	if (Container[_index] != nullptr)
		Prev = Unregister(_index);

	Container[_index] = _consumableItem;
	OnQuickSlotChanged.Broadcast(_index, Container[_index]);

	return Prev;
}

TObjectPtr<UConsumableItem> UQuickSlot::Unregister(uint8 _index)
{
	if (Container.Num() <= _index)
		return nullptr;

	TObjectPtr<UConsumableItem> Prev = Container[_index];
	Container[_index] = nullptr;
	OnQuickSlotChanged.Broadcast(_index, Prev);

	return Prev;
}

void UQuickSlot::UseItem(uint8 _index)
{
	if (Container.Num() <= _index || Container[_index] == nullptr)
		return;

	Container[_index]->Consume();
}

