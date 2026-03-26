// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWInventory.h"
#include "Components/WrapBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Define/Enum.h"
#include "Data/ItemData.h"
#include "Item/Item.h"

#include "UI/UserWidget/UWCategory.h"
#include "UI/UserWidget/UWItemSlot.h"
#include "UI/UserWidget/UWItemDetail.h"

void UUWInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CurCategory = EItemType::WEAPON;

	CloseButton->OnClicked.AddDynamic(this, &UUWInventory::HideUI);
	ThrowButton->OnClicked.AddDynamic(this, &UUWInventory::ClickThrowItem);
	EquipButton->OnClicked.AddDynamic(this, &UUWInventory::ClickEquipItem);
	UnequipButton->OnClicked.AddDynamic(this, &UUWInventory::ClickUnequipItem);
	ItemCategory->OnSelected.AddUObject(this, &UUWInventory::ClickCategory);

	ComparedItemDetail->SetVisibility(ESlateVisibility::Hidden);

	OptionalIndex = -1;
}

void UUWInventory::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);
	ShowSelectedItemDetail(false);

	if (OptionalIndex < 0) // 일반 인벤토리 열기
	{
		ItemCategory->SetVisibility(ESlateVisibility::Visible);
		ItemCategory->SetSelectElementManually(static_cast<uint8>(CurCategory));
		UpdateCategory(CurCategory, false); // 외부요인으로 변경된 카테고리일 수 있으므로 UI에 반영
	}
	else // 선택모드 일땐 끄기
		ItemCategory->SetVisibility(ESlateVisibility::Hidden);

	UpdateSlot();
}

void UUWInventory::SetSelectOption(EItemType _itemType, TWeakObjectPtr<UItem> _item, uint8 _optionalIdx)
{
	CurCategory = _itemType;
	OptionalIndex = _optionalIdx;

	if (_item.IsValid())
	{
		ComparedItemDetail->SetDetail(_item);
		ComparedItemDetail->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUWInventory::HideUI()
{
	Super::HideUI();
	OptionalIndex = -1;
	ComparedItemDetail->SetVisibility(ESlateVisibility::Hidden);
}

void UUWInventory::Init(uint8 _initSize, uint32 _gold, FGetItemArrFunc&& _func)
{
	GetItemArrFunc = _func;

	check(ItemSlotClass); // 없는 경우 크래시

	ItemSlots.SetNum(_initSize);
	for (int i = 0; i < _initSize; ++i)
	{
		ItemSlots[i] = CreateWidget<UUWItemSlot>(this, ItemSlotClass);
		ItemSlots[i]->Init(i);
		ItemSlots[i]->SetSize(SlotSize);
		ItemSlots[i]->OnSlotClicked.BindUObject(this, &UUWInventory::OnSlotClicked);

		SlotContainer->AddChild(ItemSlots[i]);
	}

	SetGoldLabel(_gold);
}

void UUWInventory::SetSlot(uint8 _idx, TWeakObjectPtr<UItem> _item)
{
	ItemSlots[_idx]->SetSlot(_item);
	ItemSlots[_idx]->MarkSelected(false);
}

void UUWInventory::SetGoldLabel(uint32 _goldValue)
{
	GoldLabel->SetText(FText::FromString(FString::FormatAsNumber(_goldValue).Append(TEXT(" G"))));
}

void UUWInventory::ClickCategory(uint8 _opt)
{
	UpdateCategory(static_cast<EItemType>(_opt));
	ShowSelectedItemDetail(false);
}

void UUWInventory::UpdateCategory(EItemType _category, bool _bUpdateSlot)
{
	CurCategory = _category;

	if (_bUpdateSlot)
		UpdateSlot();
}

void UUWInventory::UpdateSlot()
{
	if (!IsValid())
		return;

	const TArray<TObjectPtr<UItem>>& ItemArr = GetItemArrFunc.Execute(CurCategory);
	for (uint8 i = 0; i < ItemArr.Num(); ++i)
		SetSlot(i, ItemArr[i]);
}

void UUWInventory::OnSlotClicked(uint8 _index)
{
	if (!IsValid())
		return;

	ItemSlots[CurSelectedSlot]->MarkSelected(false);
	CurSelectedSlot = _index;
	
	const TArray<TObjectPtr<UItem>>& ItemArr = GetItemArrFunc.Execute(CurCategory);
	if (ItemArr[_index] != nullptr)
	{
		ItemSlots[CurSelectedSlot]->MarkSelected(true);
		SelectedItemDetail->SetDetail(ItemArr[_index]);
		ShowSelectedItemDetail(true);
	}
	else
		ShowSelectedItemDetail(false);
}

void UUWInventory::ShowSelectedItemDetail(bool _bShow)
{
	SelectedItemDetail->SetVisibility(_bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	ItemOptionContainer->SetVisibility(_bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if ((CurCategory == EItemType::ITEM) ||
		(CurCategory == EItemType::CONSUMABLE && OptionalIndex < 0))
	{
		EquipButton->SetVisibility(ESlateVisibility::Collapsed);
		UnequipButton->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (ItemSlots[CurSelectedSlot]->IsEquipped())
	{
		ThrowButton->SetVisibility(ESlateVisibility::Collapsed);
		EquipButton->SetVisibility(ESlateVisibility::Collapsed);
		UnequipButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ThrowButton->SetVisibility(ESlateVisibility::Visible);
		EquipButton->SetVisibility(ESlateVisibility::Visible);
		UnequipButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUWInventory::ClickThrowItem()
{
	if (ItemSlots[CurSelectedSlot]->IsEquipped())
		return;

	OnThrowButtonClicked.ExecuteIfBound(CurCategory, CurSelectedSlot);
	OnSlotClicked(CurSelectedSlot);
}

void UUWInventory::ClickEquipItem()
{
	OnEquipButtonClicked.ExecuteIfBound(CurCategory, CurSelectedSlot);
	UpdateSlot();
	OnSlotClicked(CurSelectedSlot);

	if (IsSubUI())
		HideUI();
}

void UUWInventory::ClickUnequipItem()
{
	OnUnequipButtonClicked.ExecuteIfBound(CurCategory, CurSelectedSlot);
	UpdateSlot();
	OnSlotClicked(CurSelectedSlot);

	if (IsSubUI())
		HideUI();
}
