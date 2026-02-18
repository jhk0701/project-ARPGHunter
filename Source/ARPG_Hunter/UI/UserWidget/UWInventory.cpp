// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWInventory.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Data/ItemData.h"
#include "Item/Item.h"
#include "UI/UserWidget/UWItemSlot.h"

void UUWInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWInventory::ClickCloseButton);
}

void UUWInventory::Init(uint8 _initSize, uint32 _gold)
{
	check(ItemSlotClass); // 없는 경우 크래시

	ItemSlots.SetNum(_initSize);
	for (int i = 0; i < _initSize; ++i)
	{
		ItemSlots[i] = CreateWidget<UUWItemSlot>(this, ItemSlotClass);
		ItemSlots[i]->Init(i, SlotSize);
		SlotContainer->AddChild(ItemSlots[i]);
	}

	SetGoldLabel(_gold);
}

void UUWInventory::SetSlot(uint8 _idx, TObjectPtr<UItem> _item)
{
	ItemSlots[_idx]->SetItem(_item);
}

void UUWInventory::SetGoldLabel(uint32 _goldValue)
{
	GoldLabel->SetText(FText::FromString(FString::FormatAsNumber(_goldValue).Append(TEXT(" G"))));
}

void UUWInventory::ClickCloseButton()
{
	HideUI();
}
