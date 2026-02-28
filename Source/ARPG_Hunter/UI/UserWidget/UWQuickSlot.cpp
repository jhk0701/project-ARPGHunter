// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWQuickSlot.h"
#include "Components/HorizontalBox.h"

#include "UI/UserWidget/UWItemSlot.h"
#include "Item/Item.h"

void UUWQuickSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	QuickSlots.SetNum(SlotContainer->GetChildrenCount());
	for (uint8 i = 0; i < SlotContainer->GetChildrenCount(); ++i)
	{
		TObjectPtr<UUWItemSlotIndicate> SlotInst = Cast<UUWItemSlotIndicate>(SlotContainer->GetChildAt(i));
		QuickSlots[i] = SlotInst;
	
		SlotInst->Init(i);
		SlotInst->SetMessage(FText::AsNumber(i + 1));
	}
}

void UUWQuickSlot::Init(const TArray<TWeakObjectPtr<UConsumableItem>>& _container)
{
	for (uint8 i = 0; i < _container.Num(); ++i)
		QuickSlots[i]->SetSlot(_container[i]);
}

void UUWQuickSlot::SetQuickSlot(uint8 _index, TWeakObjectPtr<UConsumableItem> _item)
{
	QuickSlots[_index]->SetSlot(_item);
}
