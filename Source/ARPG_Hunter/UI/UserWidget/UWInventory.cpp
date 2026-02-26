// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWInventory.h"
#include "Components/WrapBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Define/Enum.h"
#include "Data/ItemData.h"
#include "Item/Item.h"
#include "UI/ContentWidget/UWCheckBox.h"
#include "UI/UserWidget/UWItemSlot.h"
#include "UI/UserWidget/UWItemDetail.h"

void UUWInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CurCategory = EItemType::WEAPON;

	CloseButton->OnClicked.AddDynamic(this, &UUWInventory::ClickCloseButton);
	ThrowButton->OnClicked.AddDynamic(this, &UUWInventory::ClickThrowItem);
	EquipButton->OnClicked.AddDynamic(this, &UUWInventory::ClickEquipItem);
	UnequipButton->OnClicked.AddDynamic(this, &UUWInventory::ClickUnequipItem);

	for (uint8 i = 0; i < CategoryContainer->GetChildrenCount(); ++i)
	{
		TObjectPtr<UUWCheckBox> CheckBox = Cast<UUWCheckBox>(CategoryContainer->GetChildAt(i));
		CheckBox->Init();
		CheckBox->OnCheckBoxChanged.AddUObject(this, &UUWInventory::ClickCategoryCheckBox);

		EItemType Type = static_cast<EItemType>(CheckBox->GetOption());
		Category.Add(Type, CheckBox);
	}
}

void UUWInventory::ShowUI(bool _bIsSubUI)
{
	Super::ShowUI(_bIsSubUI);

	UpdateSlot();
	ShowSelectedItemDetail(false);

	if (OptionalIndex < 0) // 일반 인벤토리 열기
		CategoryContainer->SetVisibility(ESlateVisibility::Visible);
	else // 선택모드 활성화
		CategoryContainer->SetVisibility(ESlateVisibility::Hidden);
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

void UUWInventory::Init(uint8 _initSize, uint32 _gold, TFunction<const TArray<TObjectPtr<UItem>>*(EItemType)> _getItemArrFunc)
{
	GetItemArrFunc = _getItemArrFunc;

	check(ItemSlotClass); // 없는 경우 크래시

	ItemSlots.SetNum(_initSize);
	for (int i = 0; i < _initSize; ++i)
	{
		ItemSlots[i] = CreateWidget<UUWItemSlot>(this, ItemSlotClass);
		ItemSlots[i]->Init(i, SlotSize);
		ItemSlots[i]->OnSlotClicked.BindUObject(this, &UUWInventory::OnSlotClicked);

		SlotContainer->AddChild(ItemSlots[i]);
	}

	SetGoldLabel(_gold);
}

void UUWInventory::SetSlot(uint8 _idx, TWeakObjectPtr<UItem> _item)
{
	ItemSlots[_idx]->SetItem(_item);
	ItemSlots[_idx]->MarkSelected(false);
}

void UUWInventory::SetGoldLabel(uint32 _goldValue)
{
	GoldLabel->SetText(FText::FromString(FString::FormatAsNumber(_goldValue).Append(TEXT(" G"))));
}

void UUWInventory::ClickCloseButton()
{
	HideUI();
}

void UUWInventory::ClickCategoryCheckBox(bool _bIsChecked, uint8 _opt)
{
	CurCategory = static_cast<EItemType>(_opt);
	
	for(const TPair<EItemType, TObjectPtr<UUWCheckBox>>& Pair : Category)
	{
		if (Pair.Key == CurCategory)
			continue;

		Pair.Value->UpdateStateWithoutEvent(false);
	}

	ShowSelectedItemDetail(false);
	UpdateSlot();
}

void UUWInventory::UpdateSlot()
{
	if (!IsValid())
		return;

	const TArray<TObjectPtr<UItem>>* ItemArr = GetItemArrFunc(CurCategory);
	for (uint8 i = 0; i < ItemArr->Num(); ++i)
		SetSlot(i, (*ItemArr)[i]);
}

void UUWInventory::OnSlotClicked(uint8 _index)
{
	if (!IsValid())
		return;

	ItemSlots[CurSelectedSlot]->MarkSelected(false);
	CurSelectedSlot = _index;
	
	const TArray<TObjectPtr<UItem>>* ItemArr = GetItemArrFunc(CurCategory);
	if ((*ItemArr)[_index] != nullptr)
	{
		ItemSlots[CurSelectedSlot]->MarkSelected(true);
		SelectedItemDetail->SetDetail((*ItemArr)[_index]);
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
		EquipButton->SetVisibility(ESlateVisibility::Collapsed);
		UnequipButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
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
