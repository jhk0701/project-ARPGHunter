// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/QuickSlot.h"
#include "Player/Inventory.h"
#include "Item/Item.h"

UQuickSlot::UQuickSlot()
{
}

void UQuickSlot::Init(uint8 _size)
{
	Container.SetNum(_size);
}

bool UQuickSlot::IsValidSlot(uint8 _index) const
{
	return Container.Num() > _index && Container[_index].IsValid();
}

void UQuickSlot::Register(uint8 _index, TWeakObjectPtr<UItem> _consumableItem)
{
	if (Container.Num() <= _index)
		return;

	TObjectPtr<UConsumableItem> ConsumableItem = Cast<UConsumableItem>(_consumableItem);
	if (nullptr == ConsumableItem)
		return;
	
	ConsumableItem->SetQuickSlotIndex(_index);

	if (Container[_index].Get() != nullptr)
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

void UQuickSlot::ClearSlot(uint8 _index)
{
	Container[_index] = nullptr;
	OnQuickSlotChanged.Broadcast(_index, Container[_index]);
}

void UQuickSlot::UseItem(uint8 _index, IEffectable* _target)
{
	if (!IsValidSlot(_index))
		return;
	
	uint8 InventoryIndex = Container[_index]->GetInventoryIndex();
	Container[_index]->Consume(_target);
	OnQuickSlotUsed.Broadcast(_index, InventoryIndex);
	OnPostQuickSlotUsed.Broadcast(_index, InventoryIndex);
}

void UQuickSlot::LoadQuickSlot(TObjectPtr<UItem> _item)
{
	TObjectPtr<UConsumableItem> Consumable = Cast<UConsumableItem>(_item);
	Register(Consumable->GetQuickSlotIndex(), Consumable);
}
