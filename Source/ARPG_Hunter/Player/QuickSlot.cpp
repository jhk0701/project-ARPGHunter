// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/QuickSlot.h"
#include "Player/Inventory.h"
#include "Item/Item.h"
#include "Interface/Effectable.h"

UQuickSlot::UQuickSlot()
{
}

void UQuickSlot::Init(TWeakObjectPtr<UInventory> _container, uint8 _size)
{
	Inventory = _container;
	Container.SetNum(_size);
}

bool UQuickSlot::IsValidSlot(uint8 _index) const
{
	return Container.Num() > _index && Container[_index] != nullptr;
}

void UQuickSlot::Register(uint8 _index, TWeakObjectPtr<UItem> _consumableItem)
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

TWeakObjectPtr<UConsumableItem> UQuickSlot::Unregister(uint8 _index)
{
	if (!IsValidSlot(_index))
		return nullptr;

	TWeakObjectPtr<UConsumableItem> Prev = Container[_index];
	Prev->SetQuickSlotIndex(-1);
	
	Container[_index] = nullptr;
	OnQuickSlotChanged.Broadcast(_index, Container[_index]);

	return Prev;
}

void UQuickSlot::UseItem(uint8 _index, IEffectable* _target)
{
	if (!IsValidSlot(_index))
		return;

}
