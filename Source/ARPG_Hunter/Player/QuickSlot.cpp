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

void UQuickSlot::Register(uint8 _index, TObjectPtr<UItem> _consumableItem)
{
	if (Container.Num() <= _index)
		return;

	TObjectPtr<UConsumableItem> ConsumableItem = Cast<UConsumableItem>(_consumableItem);
	if (nullptr == ConsumableItem)
		return;
	
	ConsumableItem->SetQuickSlotIndex(_index);

	if (Container[_index] != nullptr)
		Unregister(_index);

	Container[_index] = ConsumableItem;
	OnQuickSlotChanged.Broadcast(_index, Container[_index]);
}

TObjectPtr<UConsumableItem> UQuickSlot::Unregister(uint8 _index)
{
	if (Container.Num() <= _index)
		return nullptr;

	TObjectPtr<UConsumableItem> Prev = Container[_index];
	Prev->SetQuickSlotIndex(-1);
	
	Container[_index] = nullptr;
	OnQuickSlotChanged.Broadcast(_index, Container[_index]);

	return Prev;
}

void UQuickSlot::UseItem(uint8 _index)
{
	if (Container.Num() <= _index || Container[_index] == nullptr)
		return;

	Container[_index]->Consume();
}

