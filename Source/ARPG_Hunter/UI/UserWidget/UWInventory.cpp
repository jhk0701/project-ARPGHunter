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

	for (uint8 i = 0; i < CategoryContainer->GetChildrenCount(); ++i)
	{
		TObjectPtr<UUWCheckBox> CheckBox = Cast<UUWCheckBox>(CategoryContainer->GetChildAt(i));
		CheckBox->Init();
		CheckBox->OnCheckBoxChanged.AddUObject(this, &UUWInventory::ClickCategoryCheckBox);

		EItemType Type = static_cast<EItemType>(CheckBox->GetOption());
		Category.Add(Type, CheckBox);
	}
}

void UUWInventory::ShowUI()
{
	Super::ShowUI();
	UpdateSlot();

	CategoryContainer->SetVisibility(ESlateVisibility::Visible);
	ShowSelectedItemDetail(false);
	ComparedItemDetail->SetVisibility(ESlateVisibility::Hidden);
}

void UUWInventory::ShowUI(EItemType _itemType, TObjectPtr<UItem> _item)
{
	Super::ShowUI();
	UpdateSlot();

	CurCategory = _itemType;
	bIsSelectMode = true;

	CategoryContainer->SetVisibility(ESlateVisibility::Hidden);
	ShowSelectedItemDetail(false);

	if (_item)
	{
		ComparedItemDetail->SetDetail(_item);
		ComparedItemDetail->SetVisibility(ESlateVisibility::Visible);
	}
	else
		ComparedItemDetail->SetVisibility(ESlateVisibility::Hidden);
}

void UUWInventory::HideUI()
{
	if (bIsSelectMode) 
	{
		RemoveFromParent();
		bIsSelectMode = false;
	}
	else 
		Super::HideUI();
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

void UUWInventory::SetSlot(uint8 _idx, TObjectPtr<UItem> _item)
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

	EquipButton->SetVisibility(CurCategory < EItemType::EQUIPABLE ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UUWInventory::ClickThrowItem()
{
	OnThrowButtonClicked.ExecuteIfBound(CurCategory, CurSelectedSlot);
	OnSlotClicked(CurSelectedSlot);
}

void UUWInventory::ClickEquipItem()
{
	OnEquipButtonClicked.ExecuteIfBound(CurCategory, CurSelectedSlot);
	OnSlotClicked(CurSelectedSlot);
}
