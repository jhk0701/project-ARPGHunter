// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWInventory.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"

#include "Data/ItemData.h"
#include "Item/Item.h"
#include "UI/UserWidget/UWItemSlot.h"

void UUWInventory::Init(const TArray<TObjectPtr<UItem>>& _items)
{
	check(ItemSlotClass); // 없는 경우 크래시

	ItemSlots.SetNum(_items.Num());
	for (int i = 0; i < _items.Num(); ++i)
	{
		ItemSlots[i] = CreateWidget<UUWItemSlot>(this, ItemSlotClass);
		ItemSlots[i]->SetItem(_items[i]); // null 일 경우 내부에서 Clear해줌
		ItemSlots[i]->SetSize(SlotSize);
		SlotContainer->AddChild(ItemSlots[i]);
	}
}

void UUWInventory::SetSlot(uint8 _idx, TObjectPtr<UItem> _item)
{
	ItemSlots[_idx]->SetItem(_item);
}

void UUWInventory::SetGoldLabel(uint32 _goldValue)
{
	GoldLabel->SetText(FText::FromString(FString::FormatAsNumber(_goldValue).Append(TEXT(" G"))));
}
